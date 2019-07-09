# Omega3D
3D flow solver with GUI using vortex particle and boundary element methods


## Overview
[Computational Fluid Dynamics (CFD)](https://en.wikipedia.org/wiki/Computational_fluid_dynamics) encompasses a wide variety of methods to aid in the numerical simulation of fluid flows on digital computers. Most methods rely on the subdivision of the fluid domain into small, stationary cells, such as tetrahedra, and solve the [Navier-Stokes equations](https://en.wikipedia.org/wiki/Navier%E2%80%93Stokes_equations) on each Eulerian (not moving) cell. In contrast, vortex methods rely on a Lagrangian (moving with the flow) description of the only the [vorticity](https://en.wikipedia.org/wiki/Vorticity)-containing region of the fluid domain and any solid boundaries present. This eliminates many of the difficulties present in traditional CFD. In addition, the form of the equations used also removes the pressure term from the Navier-Stokes equations, which is a large source of instability and extra effort in traditional CFD. This is why many new flow solvers for unsteady momentum-dominated flows (non-microscopic in scale) are implemented using vortex methods.

Omega3D aims to be an accurate combined Lagrangian-Eulerian fluid flow solver for unsteady flows with complex boundaries, with a greatly reduced reliance on meshing and parameter tuning for achieving accuracy and speed.


## Build the software
This code uses some C++17 features, so should compile on GCC 7, Clang 4, and MSVC 19.10 (Visual Studio 15 2017) compilers.

#### Prerequisites
Both the GUI and batch versions require CMake, Eigen, and GLFW version 3 to compile. To build the GUI version, users will also need GLFW3. These can be installed in Red Hat/Fedora with

    sudo dnf install cmake glfw3-devel eigen3-devel

or on Ubuntu with

    sudo apt-get install cmake glfw3-dev libeigen3-dev

or on OSX via [Homebrew](https://docs.brew.sh/Installation) with

    brew install cmake glfw eigen

#### Optional libraries
[Vc](https://github.com/VcDevel/Vc) is a vectorization library, and Omega3D uses it to greatly accelerate the velocity evaluations. This package can be built and installed external to Omega3D with

    git clone https://github.com/VcDevel/Vc.git
    cd Vc
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/opt/Vc -DBUILD_TESTING=OFF ..
    make -j 4
    sudo make install
    cd ../..

#### Compile
Upon installation of the prerequisites, the following commands should build Omega3D.

    git clone git@github.com:Applied-Scientific-Research/Omega3D.git
    cd Omega3D
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DUSE_OMP=ON -DUSE_VC=OFF ..
    make

If you were able to build and install Vc, then you should set `-DUSE_VC=ON` in the above `cmake` command.

Finally, you can run the GUI program with

    ./Omega3D.bin

and the batch program with

    ./Omega3Dbatch.bin

## To do
Tasks to consider or implement:

* Augment BEM to make it work for rotating bodies
* Get BEM working for moving bodies
* Allow general spheres - use the ips to scale refinement of an icosahedron
* Add some pics, maybe aGIF, to this readme
* Instead of manipulating the projection matrix, have the mouse change the view matrix (assume model matrix is unity), see [here](https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/) for a nice write-up on the three OpenGL matrices
* Add arcball rotation to the viewport - see [here](https://www.3dgep.com/understanding-the-view-matrix/) for some glm code
* Use [libigl](https://github.com/libigl/libigl/) or [OpenMesh](http://openmesh.org/intro/) to load geometry files for boundaries
* Start fresh GUI main file, look for first run and splash a help window
* Add other repos as submodules, like [Vc](https://github.com/VcDevel/Vc) and [nlohmann/json](https://github.com/nlohmann/json) and [libigl](https://github.com/libigl/libigl/), or just by copying? `submodule add https://...xxx.git thirdparty/xxx`
* ~~Get basic BEM working for non-moving bodies~~
* ~~Port over variable-core-size drawing techniques from O2D~~
* ~~Use the actual core function to draw the blobs - but what is the real core function?~~ NO
* ~~Allow reading obj/stl/off/ply file using `igl/read_triangle_mesh.h`~~
* ~~Draw the triangular elements, even if very basically~~
* ~~Keep updated with Omega2D development and features: vtk, png, json, etc.~~
* ~~Move radius from ElementBase into Points~~
* ~~Add merging operation to avoid over-resolution~~
* ~~Add particle splitting to avoid under-resolution~~
* ~~Add a check on stretch and pause simulation if it goes too far in one step~~
* ~~Make a thick-cored vortex ring~~
* ~~Pull in NNLS VRM algorithm from o2d, put in our Diffusion class - no amr yet~~
* ~~Add second order convection - in a class?~~
* ~~Rework the particle shader to read from all the various arrays - goal is to draw an inviscid sim~~
* ~~Rework the updateGL code to move all the appropriate arrays to the GPU~~
* ~~Make the reset button actally reset~~
* ~~Get new features to just add particles to the current Points object~~
* ~~Have the GUI set up a vortex ring object~~
* ~~Get it to run a vortex ring without drawing anything~~
* ~~Copy the simulation time step logic from Omega2D (using std::async)~~
* ~~Move time step code into Simulation/Convection files, like Omega2D~~
* ~~Make separate batch and GUI main files and binaries~~
* ~~Support CMake with optional OpenMP and optional Vc~~
* ~~Wrap solver in a GUI - use existing code from [Omega2D](https://github.com/Applied-Scientific-Research/Omega2D). This means supporting only, say, vortex rings and stretch, but no diffusion.~~
* ~~Rework into struct of arrays to allow Vc to work properly (this requires more work for compute and draw shaders :one buffer per array), but should be nice and flexible - and easier than all the `4*idx+3` crap~~
* ~~Test speed and accuracy with Vc as the float type~~
* ~~move rand() to std::random (see Omega2D)~~

## Thanks
This project is funded by the [National Institutes of Health (NIH)](https://www.nih.gov/) under grant number 1 R01 EB022180-01A1 ("A Fast High-Order CFD for Turbulent Flow Simulation in Cardio-Devices").

Thanks to [Omar Cornut](http://www.miracleworld.net/) for his [dear imgui](https://github.com/ocornut/imgui) library, file browser dialogs from [Imgui-IGS-Snippets](https://github.com/gileoo/Imgui-IGS-Snippets), sol-prog's [OpenGL Tutorials](https://github.com/sol-prog/OpenGL-101), and Jim Susinno's [OpenGL-Boilerplate](https://github.com/jimbo00000/OpenGL-Boilerplate). Reading triangle mesh files was easy with [libigl](https://github.com/libigl/libigl/).

VRM code is functional thanks to jlblancoc for [Nanoflann](https://github.com/jlblancoc/nanoflann) (a header-only tree search library), and to all of the developers of [Eigen](http://eigen.tuxfamily.org/) (a C++ matrix/vector library). The BEM code also relies heavily on [Eigen](http://eigen.tuxfamily.org/). We also love [Vc](https://github.com/VcDevel/Vc), an excellent SIMD library by Matthias Kretz.

JSON reading and writing is thanks to [JSON for Modern C++](https://github.com/nlohmann/json) by [Niels Lohmann](http://nlohmann.me). XML output to VTK files is done using [tinyxml2](https://github.com/leethomason/tinyxml2) and [cppcodec](https://github.com/tplgy/cppcodec) for base64 encoding. And mathematical expression parsing came from [Lewis Van Winkle](https://codeplea.com/)'s [tinyexpr](https://github.com/codeplea/tinyexpr).

Many thanks to NBL for valuable discussions of architecture and C++ syntax and idioms.
