/*
 * ElementPacket.h - Pass around fundamental geometry
 *
 * (c)2018-20 Applied Scientific Research, Inc.
 *            Mark J Stock <markjstock@gmail.com>
 */

#pragma once

#include "Omega3D.h"

#include <vector>

// Helper class for passing arbitrary elements around
template<class S>
struct ElementPacket {
  ElementPacket<S>() = default;
  ~ElementPacket<S>() = default;

  ElementPacket<S>(ElementPacket<S> const&) = default; //allow copy
  ElementPacket<S>(ElementPacket<S>&&) = default; //allow move
  ElementPacket<S>& operator=(ElementPacket<S> const&) = default; //allow copy
  ElementPacket<S>& operator=(ElementPacket<S>&&) = default; //allow move

  std::vector<S> x;
  std::vector<Int> idx;
  std::vector<S> val;
};

