/*
 * MeasureFeature.h - GUI-side descriptions of flow measurement features
 *
 * (c)2018-9 Applied Scientific Research, Inc.
 *           Written by Mark J Stock <markjstock@gmail.com>
 */

#pragma once

#include "json/json.hpp"

#include <iostream>
#include <vector>

//
// Abstract class for any measurement feature (streamlines, rakes, tracers, etc.) present initially
//
class MeasureFeature {
public:
  explicit
  MeasureFeature(float _x, float _y, float _z, bool _moves)
    : m_x(_x),
      m_y(_y),
      m_z(_z),
      m_is_lagrangian(_moves)
    {}

  bool moves() const { return m_is_lagrangian; }
  virtual void debug(std::ostream& os) const = 0;
  virtual std::string to_string() const = 0;
  virtual nlohmann::json to_json() const = 0;
  virtual std::vector<float> init_particles(float) const = 0;
  virtual std::vector<float> step_particles(float) const = 0;

protected:
  float m_x;
  float m_y;
  float m_z;
  bool  m_is_lagrangian;
};

std::ostream& operator<<(std::ostream& os, MeasureFeature const& ff);


//
// types of measurement features:
//
// single origin point, continuous tracer emitter
// single set of tracer particles
// fixed set of field points
// periodic rake tracer emitter
// grid of fixed field points
// solid block (square, circle) of tracers
// single streamline (save all positions of a single point, draw as a line)
//



//
// Concrete class for a single measurement point
//
class SinglePoint : public MeasureFeature {
public:
  SinglePoint(float _x, float _y, float _z, bool _moves)
    : MeasureFeature(_x, _y, _z, _moves)
    {}

  void debug(std::ostream& os) const override;
  std::string to_string() const override;
  nlohmann::json to_json() const override;
  std::vector<float> init_particles(float) const override;
  std::vector<float> step_particles(float) const override;

protected:
  //float m_str;
};


//
// Concrete class for an immobile particle emitter (one per frame)
//
class TracerEmitter : public SinglePoint {
public:
  TracerEmitter(float _x, float _y, float _z)
    : SinglePoint(_x, _y, _z, false)
    {}

  void debug(std::ostream& os) const override;
  std::string to_string() const override;
  nlohmann::json to_json() const override;
  std::vector<float> init_particles(float) const override;
  std::vector<float> step_particles(float) const override;

protected:
  // eventually implement frequency, but for now, once per step
  //float m_frequency;
};


//
// Concrete class for a circle of tracer points
//
class TracerBlob : public SinglePoint {
public:
  TracerBlob(float _x, float _y, float _z, float _rad)
    : SinglePoint(_x, _y, _z, true),
      m_rad(_rad)
    {}

  void debug(std::ostream& os) const override;
  std::string to_string() const override;
  nlohmann::json to_json() const override;
  std::vector<float> init_particles(float) const override;
  std::vector<float> step_particles(float) const override;

protected:
  float m_rad;
};


//
// Concrete class for a tracer line
//
class TracerLine : public SinglePoint {
public:
  TracerLine(float _x, float _y, float _z, float _xf, float _yf, float _zf)
    : SinglePoint(_x, _y, _z, true),
      m_xf(_xf),
      m_yf(_yf),
      m_zf(_zf)
    {}

  void debug(std::ostream& os) const override;
  std::string to_string() const override;
  nlohmann::json to_json() const override;
  std::vector<float> init_particles(float) const override;
  std::vector<float> step_particles(float) const override;

protected:
  float m_xf, m_yf, m_zf;
};


//
// Concrete class for a line of static measurement points
//
class MeasurementLine : public SinglePoint {
public:
  MeasurementLine(float _x, float _y, float _z, float _xf, float _yf, float _zf)
    : SinglePoint(_x, _y, _z, false),
      m_xf(_xf),
      m_yf(_yf),
      m_zf(_zf)
    {}

  void debug(std::ostream& os) const override;
  std::string to_string() const override;
  nlohmann::json to_json() const override;
  std::vector<float> init_particles(float) const override;
  std::vector<float> step_particles(float) const override;

protected:
  float m_xf, m_yf, m_zf;
};

