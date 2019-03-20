/*
 * Simulation.h - a class to control a 3d vortex particle sim
 *
 * (c)2017-9 Applied Scientific Research, Inc.
 *           Written by Mark J Stock <markjstock@gmail.com>
 */

#pragma once

#include "Omega3D.h"
//#include "Body.h"
#include "Collection.h"
//#include "BEM.h"
#include "Convection.h"
#include "Diffusion.h"

#ifdef USE_GL
#include "RenderParams.h"
#endif

#include <string>
#include <vector>
#include <future>
#include <chrono>

#ifdef USE_VC
#define STORE float
#define ACCUM float
#else
#define STORE float
#define ACCUM double
#endif

template <class T>
bool is_future_ready(std::future<T> const& f) {
    if (!f.valid()) return false;
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

//
// A set of particles, can be sources or targets
//
class Simulation {
public:
  Simulation();

  // imgui needs access to memory locations
  float* addr_re();
  float* addr_dt();
  float* addr_fs();

  // get the derived parameters
  float get_ips();
  float get_hnu();
  float get_vdelta();
  float get_time();
  float get_end_time();
  bool using_end_time();
  size_t get_max_steps();
  bool using_max_steps();
  float get_output_dt();
  std::string get_description();

  // setters
  void set_description(const std::string);
  void set_end_time(const double);
  void set_max_steps(const size_t);
  void set_output_dt(const double);

  // get runtime status
  //size_t get_npanels();
  size_t get_nparts();
  size_t get_nfldpts();

  // inviscid case needs this
  void set_re_for_ips(float);

  // receive and add a set of particles
  void add_particles(std::vector<float>);
  void add_fldpts(std::vector<float>, const bool);
  //void add_boundary(bdryType, std::vector<float>);

  // act on stuff
  //void set_amr(const bool);
  void set_diffuse(const bool);
  const bool get_diffuse();
  void reset();
  void async_step();
  void step();
  bool is_initialized();
  void set_initialized();
  std::string check_simulation(const size_t);
  bool test_for_new_results();
  void write_vtk();

#ifdef USE_GL
  // graphics pass-through calls
  void initGL(std::vector<float>&, float*, float*, float*);
  void updateGL();
  void drawGL(std::vector<float>&, RenderParams&);
#endif

private:
  // primary simulation params
  float re;
  float dt;
  float fs[Dimensions];

  // List of independent rigid bodies (and one for ground)
  //std::vector< std::shared_ptr<Body> > bodies;

  // Object to contain all Lagrangian elements
  std::vector<Collection> vort;		// active elements

  // Object to contain all Reactive elements
  //   inside is the vector of bodies and inlets and liftinglines/kuttapoints
  //   and the Panels list of all unknowns discretized representations
  std::vector<Collection> bdry;		// reactive-active elements like BEM surfaces

  // Object with all of the non-reactive, non-active (inert) points
  std::vector<Collection> fldpt;	// tracers and field points

  // The need to solve for the unknown strengths of reactive elements inside both the
  //   diffusion and convection steps necessitates a BEM object here
  //BEM<STORE,Int> bem;

  // Diffusion will resolve exchange of strength among particles and between panels and particles
  // Note that NNLS needs doubles for its compute type or else it will fail
  //   but velocity evaluations using Vc need S=A=float
  Diffusion<STORE,ACCUM,Int> diff;

  // Convection class takes bodies, panels, and vector of Particles objects
  //   and performs 1st, 2nd, etc. RK forward integration
  //   inside here are non-drawing Particles objects used as temporaries in the multi-step methods
  //   also copies of the panels, which will be recreated for each step, and solutions to unknowns
  //   Template parameter is accumulator type (float is OK)
  Convection<float> conv;

  // state
  std::string description;
  double time;
  double output_dt;
  double end_time;
  bool use_end_time;
  size_t max_steps;
  bool use_max_steps;
  bool sim_is_initialized;
  bool step_has_started;
  bool step_is_finished;
  std::future<void> stepfuture;  // this future needs to be listed after the big four: diff, conv, ...
};

