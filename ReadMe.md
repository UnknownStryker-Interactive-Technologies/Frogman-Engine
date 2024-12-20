[![License](https://img.shields.io/badge/License-apache-red.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-x86_64-white.svg)](PLATFORM)

# Frogman-Engine
Copyright © from 2022-present, UNKNOWN STRYKER. All Rights Reserved.  
Frogman Engine is licensed under the Apache License, version 2.0.  
This project is under development and may contain some bugs and issues.

# Frogman Engine Coding Conventions and Rules.
https://savory-moth-a00.notion.site/5cef12dd2c9b43588f22045405e80cff?v=86ea5413ff4f40de9204c4aa22907ffc&pvs=4

# Prerequisites: 
1. CMake 3.25.0 or the latest  
2. The latest Visual Studio 2022 with Windows SDK version 10.0.26100.0 

# C++ standard version: 
C++ 20 or the latest.  
All project settings can be adjusted by modifying CMakeLists.txt.

# The current development status.
| Platform     | Architecture  | Status           |
|--------------|---------------|------------------|
| Windows 11   | X86-64        | In Development   |
| Ubuntu-Linux | X86-64        | Discontinued     |

# This project leverages:

- Boost Libraries 1.86.0  
https://github.com/boostorg/boost/releases/tag/boost-1.86.0  

- City Hash  
https://github.com/google/cityhash  

- GLFW 3.4  
https://www.glfw.org/download.html  

- GLM 1.0.1  
https://github.com/g-truc/glm/releases/tag/1.0.1  

- Dear ImGUI 1.91.6  
https://github.com/ocornut/imgui/releases/tag/v1.91.6  

- Robin Hood Hash 3.11.5  
https://github.com/martinus/robin-hood-hashing  

- Task Flow 3.8.0  
https://github.com/taskflow/taskflow/releases/tag/v3.8.0  

- Google Test 1.14.0  
https://github.com/google/googletest/releases/tag/v1.14.0  

- Google Benchmark 1.8.4  
https://github.com/google/benchmark/releases/tag/v1.8.4  

# Help:
Issues with Frogman Engine Header Tool:  
- It is highly probable that the file or path string encoding issue is causing the problem.  
- The header tool requires header files and the copy of a license text file to be encoded with UTF8 with BOM(Byte Order Mark) signature.  

To run the header tool with CMake, call the CMake function:
```CMake
# The first argument is the header file paths, and the second argument is the options to the tool. 
# The tool will not properly work without wrapping " " around the each argument.  
# The each header file path and the each option must be seperated with a semi-colon ';'.  
RUN_HEADER_TOOL("${FE_LOG_HEADERS};${FE_POOL_HEADERS};${FE_CORE_HEADERS};${FE_MISC_HEADERS}" "-max-concurrency=8;-path-to-copyright-notice=${FE_CORE_CMAKE_CURRENT_LIST_DIR}/../../LICENSE.txt;-fno-reflection-helper")  
```

Microsoft Visual Studio C++ Runtime Library:
- The default settings use /MTd (Static C++ Standard Library Debug Build) for debug configuration and /MT (Static C++ Standard Library Release Build) for release.  

To run Google Benchmarks on Windows:
```CMake
LINK_LIBRARIES(advapi32.lib shlwapi.lib) # to use Google Benchmark on Windows.  
```

In order to build boost libraries using Microsoft Visual Studio 2022 Clang CL (this does not work with Windows SDK version 10.0.26100.0):
1. Download the LLVM Clang CL from Visual Studio Installer.  
2. Download Boost libraries from https://www.boost.org/  
3. Build b2.exe by running bootstrap.bat(.sh)  
4. Run the commands:  
```commands
./b2 toolset=clang-win architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=debug  
./b2 toolset=clang-win architecture=x86 address-model=64 link=static runtime-link=static threading=multi variant=release  
```