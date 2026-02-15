# Generals Mobile Port

This project is a dedicated fork of the [TheSuperHackers/GeneralsGameCode](https://github.com/TheSuperHackers/GeneralsGameCode) repository. We would like to extend our sincere gratitude and a major shoutout to the original developers and the community at TheSuperHackers for their monumental work in modernizing the *Command & Conquer: Generals* and *Zero Hour* codebase to C++20 and Visual Studio 2022.

## Project Objective

The primary goal of this fork is to port the modernized Sage engine to mobile platforms, with a current focus on **Android**. This effort involves:

- Adapting the codebase for ARM64 and other mobile architectures.
- Implementing mobile-friendly Hardware Abstraction Layers (HAL).
- Optimizing engine performance for mobile chipsets.
- Ensuring cross-platform compatibility by removing or stubbing legacy Windows-specific dependencies.

## Recent Progress and Updates

We have made significant strides in preparing the codebase for mobile environments:

- **Android Toolchain Integration**: Added support for Android NDK and specialized toolchains (`android.cmake`).
- **Build System Modernization**: Refactored the CMake configuration and presets to support cross-compilation and handle dependencies more robustly.
- **Legacy Component Refactoring**: Stubbed out Windows-only systems such as DirectX 8 wrappers and the original Recorder system to facilitate compilation on non-Windows platforms.
- **Security & Integrity**: Performed an initial security audit and code cleanup to maintain a high-quality codebase.

## Acknowledgments

This project is built upon the foundation laid by the [TheSuperHackers](https://github.com/TheSuperHackers) team. Their work in transitioning the game from its original C++98/VS6 roots to a modern, maintainable state is what made this mobile porting project possible.

## Building the Project

Support is provided for building on Windows, Linux (via Docker), and now targeting Android.

### Android Build (Experimental)

To build for Android, ensure you have the Android NDK installed and configured in your environment. Detailed instructions on using the mobile toolchains can be found in the `scripts` or `Android` directories.

### Original Build Instructions

For standard Windows and Linux builds, please refer to the original build guides or use the following quick start commands:

**Windows (Visual Studio 2022)**
```bash
cmake --preset win32
cmake --build build/win32 --config Release
```

**Linux (via Docker)**
```bash
./scripts/docker-build.sh
```

---

*Note: This project is a community effort and is not endorsed by or affiliated with EA.*
