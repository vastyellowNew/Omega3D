# Omega3D
High-performance 3D computational fluid dynamics solver with easy GUI

## Overview

Omega3D aims to be an accurate combined Lagrangian-Eulerian fluid flow solver for unsteady flows with complex boundaries, with a greatly reduced reliance on meshing and parameter tuning for achieving accuracy and speed.

It currently contains a mere architectural skeleton onto which more complex behavior can be built.

## Build and run

    git clone git@github.com:Applied-Scientific-Research/Omega3D.git
    cd Omega3D/src
    make
    ./Omega3D.bin

## To do

* Copy the simulation time step logic from Omega2D (using std::async)
* Use [libigl](https://github.com/libigl/libigl/) or something else to load geometry files for boundaries
* Have the GUI set up a vortex ring object
* Start fresh GUI main file, look for first run and splash a help window
* Move time step code into Simulation/Convection files, like Omega2D
* ~~Make separate batch and GUI main files and binaries~~
* ~~Support CMake with optional OpenMP and optional Vc~~
* Wrap solver in a GUI - use existing code from [Omega2D](https://github.com/Applied-Scientific-Research/Omega2D). This means supporting only, say, vortex rings and stretch, but no diffusion.
* ~~Rework into struct of arrays to allow Vc to work properly (this requires more work for compute and draw shaders :one buffer per array), but should be nice and flexible - and easier than all the `4*idx+3` crap~~
* Test ~~speed and~~ accuracy with Vc as the float type
* ~~move rand() to std::random (see Omega2D)~~
* Add other repos as submodules, like [Vc](https://github.com/VcDevel/Vc) and [nlohmann/json](https://github.com/nlohmann/json) and [libigl](https://github.com/libigl/libigl/), or just by copying?

        submodule add https://...xxx.git thirdparty/xxx

## Thanks

Many thanks to NBL for valuable discussions of architecture and C++ syntax and idioms.
