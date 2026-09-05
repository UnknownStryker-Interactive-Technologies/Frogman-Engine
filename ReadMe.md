[![License](https://img.shields.io/badge/License-Frogman_Engine-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-AMD64-red.svg)](PLATFORM)
[![SIMD](https://img.shields.io/badge/SIMD-AVX/AVX2-blue.svg)](PLATFORM)

# Frogman-Engine™
**LICENSE**: https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

### Frogman Engine Web Page
https://savory-moth-a00.notion.site/Frogman-Engine-1735fa4fb82e800e8fccc8df394eec5b

https://github.com/user-attachments/assets/81ea57c7-0b17-45bd-bfd0-a50467f7bd56

## Frogman Engine GDK Utilities  
- Frogman Engine Installer: https://github.com/UnknownStryker-Interactive-Technologies/Installer  
- Frogman Engine Launcher: https://github.com/UnknownStryker-Interactive-Technologies/Launcher
- Frogman Engine VSIX for Visual Studio 2022/2026: https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-VSIX-for-Visual-Studio-2022
  
If you prefer installing and setting up the engine with just a few clicks, those  tools will be the right choice for you to get started. Please send me an email to **unknownstryker416@gmail.com** if you finds some bugs or the installer does not properly download or build libraries on your system.

https://github.com/user-attachments/assets/1bf9b5c9-dbbc-405d-8df5-5e0538f4d89c

## Project Requirements: 
- MSVC C++ 20  
- CMake 4.2.0  
- SSE, SSE2, AVX, and AVX2 Support  
- Windows SDK 10.0.17134.0 or later (for the engine runtime)
- Windows 11 AMD64 System

## The current development status.
| Platform     | Architecture  | Status           |
|--------------|---------------|------------------|
| Windows 11   | X86-64        | In Development   |
| Android      | ARM64         | Pending          |

## This project leverages:
- abseil 20260107.1  
https://github.com/abseil/abseil-cpp/releases/tag/20260107.1

- Boost Libraries 1.91.0  
https://github.com/boostorg/boost/releases/tag/boost-1.91.0  

- City Hash  
https://github.com/google/cityhash  

- Dear ImGUI 1.91.6  
https://github.com/ocornut/imgui/releases  

- Dr Libs  
https://github.com/mackron/dr_libs/tree/master?tab=readme-ov-file  

- EnTT 4.0.0  
https://github.com/skypjack/entt

- GLFW 3.4  
https://www.glfw.org/download.html  

- GLM 1.0.3  
https://github.com/g-truc/glm/releases

- HAT Trie 0.7.1  
https://github.com/Tessil/hat-trie  

- lz4 1.10.0  
https://github.com/lz4/lz4/releases/tag/v1.10.0  

- stb image  
https://github.com/nothings/stb  

- Task Flow 4.0.0  
https://github.com/taskflow/taskflow/releases  

- ufbx 0.22.0  
https://github.com/ufbx/ufbx  

- Google Test 1.16.0  
https://github.com/google/googletest/releases/tag/v1.16.0  

- Google Benchmark 1.9.1  
https://github.com/google/benchmark/releases/tag/v1.9.1  




# Benchmarks
### Memory Allocator Benchmarks
<img width="2480" height="6050" alt="20256 09 05 allocator benchmarks_dark" src="https://github.com/user-attachments/assets/55f22ada-b7db-489d-9b95-9f59f28224b7" />




# The Frogman Engine GDK Architecture
### Current
<img width="680" height="560" alt="Current" src="https://github.com/user-attachments/assets/03d3f380-ab46-4ad7-9e81-b60a5d3fd7e9" />



### Planned
<img width="680" height="920" alt="Planned" src="https://github.com/user-attachments/assets/2b0c6299-e101-4ba2-b878-b920b7c017e5" />

# ~~Research Papers~~
~~The Frogman Engine Runtime Reflection System: https://1drv.ms/w/c/1b09399108712a7a/IQBmG2Q_gzsIQ7W7kQiyZHB9AZ7pBBd6sCWoF7OvIwkwVUo~~




# Help:
Issues with Frogman Engine Header Tool:  
- It is highly probable that the file or path string encoding issue is causing the problem.  
- The header tool requires header files and the copy of a license text file to be encoded with UTF8 with BOM(Byte Order Mark) signature.  

To run the header tool with CMake, call this CMake function:
```CMake
# The first argument is the header files' paths, and the latter ones are the options to the tool. 
# The tool will not properly work without wrapping " " around the header files paths argument.  
# The each header file path must be seperated with a semi-colon ';'.  
RUN_FROGMAN_HEADER_TOOL(${FE_LOG_HEADERS};${FE_POOL_HEADERS};${FE_CORE_HEADERS};${FE_MISC_HEADERS} -max-concurrency=8 -path-to-copyright-notice=${FE_CORE_CMAKE_CURRENT_LIST_DIR}/../../LICENSE.txt )  
```

How to build an application with Frogman Engine using CMake?
```CMake
ADD_FROGMAN_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# To use ADD_EXECUTABLE() instead of the function above, generated.cpp has to be added to the source files list.
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp)
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
