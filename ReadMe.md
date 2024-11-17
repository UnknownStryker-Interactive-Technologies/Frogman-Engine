[![License](https://img.shields.io/badge/License-apache-red.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-x86_64-white.svg)](PLATFORM)

# Frogman-Engine
Copyright © from 2022-present, UNKNOWN STRYKER. All Rights Reserved.  
Frogman Engine is licensed under the Apache License, version 2.0.  
This project is under development and may contain some bugs and issues.

# Frogman Engine Coding Conventions and Rules.
https://savory-moth-a00.notion.site/5cef12dd2c9b43588f22045405e80cff?v=86ea5413ff4f40de9204c4aa22907ffc&pvs=4

# Prerequisites: 
1. CMake 3.27.0 or the latest  
2. The latest Visual Studio 2022 with Windows SDK version 10.0.26100.0 

# C++ standard version: 
C++ 20 or the latest.  
All project settings can be adjusted by modifying CMakeLists.txt.

# The current development status.
| Platform     | Architecture  | Status           |
|--------------|---------------|------------------|
| Windows 11   | X86-64        | In Development   |
| Ubuntu-Linux | X86-64        | Discontinued     |

# This project uses:
- GLFW 3.4
https://www.glfw.org/download.html

- GLM 1.0.1
https://github.com/g-truc/glm/releases/tag/1.0.1

- Boost Libraries 1.86.0
https://github.com/boostorg/boost/releases/tag/boost-1.86.0

- Google Test 1.14.0
https://github.com/google/googletest/releases/tag/v1.14.0

- Google Benchmark 1.8.4
https://github.com/google/benchmark/releases/tag/v1.8.4

# Help:
The default settings use /MTd (Static C++ Standard Library Debug Build) for debug configuration and /MT (Static C++ Standard Library Release Build) for release.

LINK_LIBRARIES(advapi32.lib shlwapi.lib) to use Google Benchmark on Windows.

In order to build boost libraries using Microsoft Visual Studio 2022 Clang CL:
1. Download the LLVM Clang CL from Visual Studio Installer.
2. Download Boost libraries from https://www.boost.org/
3. Build b2.exe by running bootstrap.bat(.sh)
4. Run the commands: 

./b2 toolset=clang-win architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=debug 

./b2 toolset=clang-win architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=release
