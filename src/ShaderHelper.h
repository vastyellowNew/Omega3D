/*
 * ShaderHelper.h - Methods for generating opengl programs
 *
 * These methods are from https://solarianprogrammer.com/2013/05/13/opengl-101-drawing-primitives/
 * and https://github.com/sol-prog/OpenGL-101
 *
 * (c)2017-9 Applied Scientific Research, Inc.
 *           Written by Mark J Stock <markjstock@gmail.com>
 */

#pragma once

// for glad
#ifndef APIENTRY
  #ifdef _WIN32
    #define APIENTRY __stdcall
  #else
    #define APIENTRY
  #endif
  #define GL_APIENTRY_DEFINED
#endif // APIENTRY

#include "glad.h"

// Create a render program from two shaders (vertex and fragment)
GLuint create_draw_blob_program();
GLuint create_draw_point_program();
GLuint create_draw_surface_tri_prog();

// Create a compute program from one shader, and a zeroing program
GLuint create_ptptvelgrad_program();
GLuint create_initvelgrad_program();
