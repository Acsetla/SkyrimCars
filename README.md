# SkyrimCars

Native SKSE/CommonLibSSE-NG vehicle project for Skyrim AE/SE runtime 1.6.1170.

## Current state

The repository now contains the native plugin foundation:

- C++23 + CommonLibSSE-NG
- runtime gate for 1.6.1170
- SKSE plugin bootstrap and logging
- keyboard input sink
- Ctrl+I vehicle toggle request
- isolated vehicle manager
- first-pass arcade handling model
- update loop ready for the real vehicle reference/physics layer

## Planned gameplay

- Ctrl+I opens the vehicle selector.
- Mercedes and Jeep are selectable.
- One active vehicle at a time.
- W/S throttle, brake and reverse.
- A/D steering.
- Space handbrake.
- E enter/exit.
- GTA-style third-person follow camera.
- Collision and Havok-backed vehicle physics.

## Important

The handling class is only the first control/kinematics layer. It does not pretend a static object is a finished car. The next milestone is the actual NIF + collision + Havok vehicle reference and driver/exit system.

Vehicle meshes will only be bundled when their licenses permit redistribution. Otherwise the project will use a documented dependency/asset workflow.

## Build requirements

- Windows 10/11 x64
- Visual Studio 2022 with Desktop C++ workload
- CMake 3.25+
- vcpkg
- SKSE64 for Skyrim runtime 1.6.1170
- Address Library for SKSE Plugins
- CommonLibSSE-NG via the Color-Glass vcpkg registry
