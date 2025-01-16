[![License](https://img.shields.io/badge/License-apache-red.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-x86_64-white.svg)](PLATFORM)

# Frogman-Engine
Copyright © from 2022-present, UNKNOWN STRYKER. All Rights Reserved.  
Frogman Engine is licensed under the Apache License, version 2.0.  
This project is under development and may contain some bugs and issues.

# Frogman Engine Website (incomplete)
Currently, programming convention is only available in Korean.  
SDK guide, API documentation, etc are not available yet.  
https://savory-moth-a00.notion.site/Frogman-Engine-1735fa4fb82e800e8fccc8df394eec5b

# Prerequisites: 
1. CMake 3.25.0 or the latest  
2. The latest Visual Studio 2022 with Windows SDK version 10.0.26100.0 
3. Microsoft Parallel Patterns Library

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

- HAT Trie 0.6.0  
https://github.com/Tessil/hat-trie  

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

To run the header tool with CMake, call this CMake function:
```CMake
# The first argument is the header files paths, and the later ones the options to the tool. 
# The tool will not properly work without wrapping " " around the header files paths argument.  
# The each header file path must be seperated with a semi-colon ';'.  
RUN_FROGMAN_HEADER_TOOL("${FE_LOG_HEADERS};${FE_POOL_HEADERS};${FE_CORE_HEADERS};${FE_MISC_HEADERS}" -max-concurrency=8 -path-to-copyright-notice=${FE_CORE_CMAKE_CURRENT_LIST_DIR}/../../LICENSE.txt )  
```

- The CMake function 'RUN_FROGMAN_HEADER_TOOL()' will generate the reflection helper code within the generated.cpp.  

This lets Frogman Engine dynamically instantiate objects without hard-coded object construction statements when reading a game configuration file to instantiate the necessary objects.
```C++
// generated.cpp
// Copyright ⓒ from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/reflection.hpp> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/FE-HT-Test/HeaderWithoutCopyright.hpp>

void load_reflection_data()
{
    ::FE::framework::framework_base::get_engine().get_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >("FHT Gen ::FrogmanEngine::Physics::Vector2()", &::FE::framework::reflection::construct_object<::FrogmanEngine::Physics::Vector2>); 
    ::FE::framework::framework_base::get_engine().get_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >("FHT Gen ::FrogmanEngine::Rendering::Renderer()", &::FE::framework::reflection::construct_object<::FrogmanEngine::Rendering::Renderer>); 
}
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