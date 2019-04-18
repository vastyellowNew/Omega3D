/*
 * Diffusion.h - a class for diffusion of strength from bodies to particles and among particles
 *
 * (c)2017-9 Applied Scientific Research, Inc.
 *           Written by Mark J Stock <markjstock@gmail.com>
 */

#pragma once

#include "Omega3D.h"
#include "Core.h"
//#include "Body.h"
#include "Merge.h"
#include "Reflect.h"
#include "VRM.h"
#include "BEM.h"

#include <cstdlib>
#include <iostream>
#include <vector>


//
// One step of convection of elements, allowing for boundary conditions
//
// templatized on 'S'torage, 'A'ccumulator, and 'I'ndex types
//
template <class S, class A, class I>
class Diffusion {
public:
  Diffusion()
    : vrm(),
      core_func(gaussian),
      is_inviscid(false),
      adaptive_radii(false),
      nom_sep_scaled(std::sqrt(8.0)),
      particle_overlap(1.5)
    {}

  const bool get_diffuse() { return !is_inviscid; }
  void set_diffuse(const bool _do_diffuse) { is_inviscid = not _do_diffuse; }
  void set_amr(const bool _do_amr) { adaptive_radii = _do_amr; }
  S get_nom_sep_scaled() const { return nom_sep_scaled; }
  S get_particle_overlap() const { return particle_overlap; }
  CoreType get_core_func() const { return core_func; }

  void step(const double,
            const double,
            const S,
            const S,
            const std::array<double,3>&,
            std::vector<Collection>&,
            std::vector<Collection>&,
            BEM<S,I>& _bem);

private:
  // the VRM algorithm, template params are storage, compute, max moments
  // note that NNLS needs doubles for its compute type or else it will fail
  VRM<S,A,2> vrm;

  // other necessary variables
  CoreType core_func;

  // toggle inviscid
  bool is_inviscid;

  // toggle adaptive particle sizes
  bool adaptive_radii;

  // nominal separation normalized by h_nu
  S nom_sep_scaled;

  // particle core size is nominal separation times this
  S particle_overlap;
};


//
// take a diffusion step
//
template <class S, class A, class I>
void Diffusion<S,A,I>::step(const double                _time,
                            const double                _dt,
                            const S                     _re,
                            const S                     _vdelta,
                            const std::array<double,3>& _fs,
                            std::vector<Collection>&    _vort,
                            std::vector<Collection>&    _bdry,
                            BEM<S,I>&                   _bem) {

  if (is_inviscid) return;

  std::cout << "Inside Diffusion::step with dt=" << _dt << std::endl;

  // always re-run the BEM calculation before shedding
  solve_bem<S,A,I>(_time, _fs, _vort, _bdry, _bem);

  //
  // generate particles at boundary surfaces
  //

  for (auto &coll : _bdry) {

    // run this step if the collection is Surfaces
    if (std::holds_alternative<Surfaces<S>>(coll)) {
      Surfaces<S>& surf = std::get<Surfaces<S>>(coll);

      // generate particles just above the surface
      std::vector<S> new_pts = surf.represent_as_particles(0.0001*(S)_dt, _vdelta);

      // add those particles to the main particle list
      if (_vort.size() == 0) {
        // no collections yet? make a new collection
        _vort.push_back(Points<S>(new_pts, active, lagrangian, nullptr));      // vortons
      } else {
        // HACK - add all particles to first collection
        auto& coll = _vort.back();
        // only proceed if the last collection is Points
        if (std::holds_alternative<Points<S>>(coll)) {
          Points<S>& pts = std::get<Points<S>>(coll);
          pts.add_new(new_pts);
        }
      }
    }

    // Kutta points and lifting lines can generate points here
  }

  //
  // diffuse strength among existing particles
  //

  // ensure that we have a current h_nu
  vrm.set_hnu(std::sqrt(_dt/_re));

  // ensure that it knows to allow or disallow adaptive radii
  vrm.set_adaptive_radii(adaptive_radii);

  // loop over active vorticity
  for (auto &coll : _vort) {

    // if no strength, skip
    if (std::visit([=](auto& elem) { return elem.is_inert(); }, coll)) continue;

    // run this step if the collection is Points
    if (std::holds_alternative<Points<S>>(coll)) {

      Points<S>& pts = std::get<Points<S>>(coll);
      std::cout << "    computing diffusion among " << pts.get_n() << " particles" << std::endl;

      // none of these are passed as const, because both may be extended with new particles
      std::array<Vector<S>,Dimensions>& x = pts.get_pos();
      Vector<S>&                        r = pts.get_rad();
      std::array<Vector<S>,Dimensions>& s = pts.get_str();

      // and make vectors for the new values
      Vector<S> newr = r;
      Vector<S> dsx, dsy, dsz;
      dsx.resize(r.size());
      dsy.resize(r.size());
      dsz.resize(r.size());

      // finally call VRM
      vrm.diffuse_all(x[0], x[1], x[2], r, newr, s[0], s[1], s[2], dsx, dsy, dsz, core_func, particle_overlap);

      // apply the strength change to the particles
      //elem->increment_in_place();
      assert(dsx.size()==s[0].size());
      for (size_t i=0; i<s[0].size(); ++i) {
        s[0][i] += dsx[i];
      }
      assert(dsy.size()==s[1].size());
      for (size_t i=0; i<s[1].size(); ++i) {
        s[1][i] += dsy[i];
      }
      assert(dsz.size()==s[2].size());
      for (size_t i=0; i<s[2].size(); ++i) {
        s[2][i] += dsz[i];
      }

      // and update the strength
      //elem->update_max_str();

      // we probably have a different number of particles now, resize the u, ug, elong arrays
      pts.resize(r.size());
    }
  }


  //
  // reflect interior particles to exterior because VRM only works in free space
  //
  (void) reflect_interior<S>(_bdry, _vort);


  //
  // merge any close particles to clean up potentially-dense areas
  //
  (void) merge_operation<S>(_vort, particle_overlap, 0.4, adaptive_radii);


  //
  // clean up by removing the innermost layer - the one that will be represented by boundary strengths
  //
  (void) clear_inner_layer<S>(_bdry, _vort, 0.0, _vdelta/particle_overlap);


  //
  // merge again
  //
  (void) merge_operation<S>(_vort, particle_overlap, 0.4, adaptive_radii);


  // now is a fine time to reset the max active/particle strength
  for (auto &coll : _vort) {
    std::visit([=](auto& elem) { elem.update_max_str(); }, coll);
  }
}

