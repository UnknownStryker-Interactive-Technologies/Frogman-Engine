[![License](https://img.shields.io/badge/License-Frogman_Engine_Apache-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-x86_64-white.svg)](PLATFORM)
[![SIMD](https://img.shields.io/badge/SIMD-AVX-blue.svg)](PLATFORM)
[![SIMD](https://img.shields.io/badge/SIMD-AVX512F-red.svg)](PLATFORM)

# Frogman-Engine
Copyright © from 2022-present, UNKNOWN STRYKER. All Rights Reserved.  
**The Frogman Engine is licensed under a modified Apache License 2.0.**  
⚠️ **NOTICE**: The contents of this repository **MAY NOT BE USED** for **training AI(Artificial Intelligence) models** without prior written permission. Please contact unknownstryker416@gmail.com to request for the permission.  
The **Frogman Engine Game Development Kit** is **TOTALLY FREE** without any licensing fees if conforming to the **License** and if used for **game development**. Otherwise license fees apply.

### Frogman Engine GDK has these two major GUI utility apps  
- Frogman Engine Installer: https://github.com/UnknownStryker-Interactive-Technologies/Installer  
- Frogman Engine Launcher: https://github.com/UnknownStryker-Interactive-Technologies/Launcher  
  
If you prefer installing and setting up the engine with just a few clicks, those  tools will be the right choice for you to get started. Please send me an email to **unknownstryker416@gmail.com** if you finds some bugs or the installer does not properly download or build libraries on your system.

<img width="6000" height="3375" alt="Frogman Engine" src="https://github.com/user-attachments/assets/393a9994-962b-4633-806d-52ec8d8ba40c" />

# Frogman Engine Website (incomplete)
Currently, programming convention is only available.  
SDK guide, API documentation, etc are unavailable yet.  
https://savory-moth-a00.notion.site/Frogman-Engine-1735fa4fb82e800e8fccc8df394eec5b

# Prerequisites: 
1. CMake 3.25.0 or the latest.  
2. The latest Visual Studio 2022.  
3. Git and Git LFS.
4. Microsoft Parallel Patterns Library.  

# C++ standard version: 
C++ 20 or the latest.  
All project settings can be adjusted by modifying CMakeLists.txt.

# The current development status.
| Platform     | Architecture  | Status           |
|--------------|---------------|------------------|
| Windows 11   | X86-64        | In Development   |
| Ubuntu-Linux | X86-64        | Discontinued     |

# This project leverages:
- abseil 20250814.1  
https://github.com/abseil/abseil-cpp/releases/tag/20250814.1  

- AES  
https://github.com/mrdcvlsc/AES

- Assimp 6.0.2  
https://github.com/assimp/assimp/releases/tag/v6.0.2  

- Boost Libraries 1.87.0  
https://github.com/boostorg/boost/releases/tag/boost-1.87.0  

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

- lz4 1.10.0  
https://github.com/lz4/lz4/releases/tag/v1.10.0

- Wwise SDK  
https://www.audiokinetic.com/en/download  

- Robin Hood Hash 3.11.5  
https://github.com/martinus/robin-hood-hashing  

- Robin Map 1.4.0  
https://github.com/Tessil/robin-map/releases/tag/v1.4.0  

- stb image  
https://github.com/nothings/stb

- Unordered Dense 4.7.0  
https://github.com/martinus/unordered_dense/releases  

- Task Flow 3.8.0  
https://github.com/taskflow/taskflow/releases/tag/v3.8.0  

- Google Test 1.16.0  
https://github.com/google/googletest/releases/tag/v1.16.0  

- Google Benchmark 1.9.1  
https://github.com/google/benchmark/releases/tag/v1.9.1  

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

- The CMake function 'RUN_FROGMAN_HEADER_TOOL()' will generate the reflection helper code within the generated.cpp.  

This lets Frogman Engine dynamically instantiate objects without hard-coded object construction statements when reading a game configuration file to instantiate the necessary objects.
```C++
// generated.cpp
// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/Unit-Tests/FE.ECS.hpp>


void serialize_component_ak_magazine(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::ak_magazine* const>(component_p), version_p);
}

void deserialize_component_ak_magazine(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::ak_magazine* const>(component_p), version_p);
}

void serialize_component_health(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::health* const>(component_p), version_p);
}

void deserialize_component_health(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::health* const>(component_p), version_p);
}

void serialize_component_weapon(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::weapon* const>(component_p), version_p);
}

void deserialize_component_weapon(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::weapon* const>(component_p), version_p);
}

void serialize_component_speed(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::speed* const>(component_p), version_p);
}

void deserialize_component_speed(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::speed* const>(component_p), version_p);
}


void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::terrorist>(::FE::ASCII* const)> >("::terrorist", &::FE::ECS::instanciate_entity<::terrorist>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::terrorist>&)> >("~::terrorist", &::FE::ECS::destruct_entity<::terrorist>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::AK47>(::FE::ASCII* const)> >("::AK47", &::FE::ECS::instanciate_entity<::AK47>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::AK47>&)> >("~::AK47", &::FE::ECS::destruct_entity<::AK47>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::ak_ammo>(::FE::ASCII* const)> >("::ak_ammo", &::FE::ECS::instanciate_entity<::ak_ammo>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::ak_ammo>&)> >("~::ak_ammo", &::FE::ECS::destruct_entity<::ak_ammo>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::player>(::FE::ASCII* const)> >("::player", &::FE::ECS::instanciate_entity<::player>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::player>&)> >("~::player", &::FE::ECS::destruct_entity<::player>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::ak_magazine>(::FE::archetype_base* const)> >("::ak_magazine", &::FE::ECS::add_component<::ak_magazine>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::ak_magazine", &::FE::ECS::remove_component<::ak_magazine>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_ak_magazine", &serialize_component_ak_magazine);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_ak_magazine", &deserialize_component_ak_magazine);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::health>(::FE::archetype_base* const)> >("::health", &::FE::ECS::add_component<::health>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::health", &::FE::ECS::remove_component<::health>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_health", &serialize_component_health);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_health", &deserialize_component_health);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::weapon>(::FE::archetype_base* const)> >("::weapon", &::FE::ECS::add_component<::weapon>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::weapon", &::FE::ECS::remove_component<::weapon>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_weapon", &serialize_component_weapon);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_weapon", &deserialize_component_weapon);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::speed>(::FE::archetype_base* const)> >("::speed", &::FE::ECS::add_component<::speed>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::speed", &::FE::ECS::remove_component<::speed>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_speed", &serialize_component_speed);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_speed", &deserialize_component_speed);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::system_view<::damage_system>()> >("::damage_system", &::FE::ECS::register_system<::damage_system>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::config2*(::config2*)> >("construct ::config2", &::std::construct_at<::config2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::config2*)> >("destruct ::config2", &::std::destroy_at<::config2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::config*(::config*)> >("construct ::config", &::std::construct_at<::config>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::config*)> >("destruct ::config", &::std::destroy_at<::config>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FrogmanEngineHeaderToolError >("::FrogmanEngineHeaderToolError",
    {
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_NoProgramOptionsAreGiven, "_FatalCmdInputError_NoProgramOptionsAreGiven" },
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_NoFilesAreGiven, "_FatalCmdInputError_NoFilesAreGiven" },
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "_FatalCmdInputError_InvalidPathToCMakeProject" },
        { ::FrogmanEngineHeaderToolError::_FatalError_FailedToOpenFile, "_FatalError_FailedToOpenFile" },
        { ::FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven, "_InputError_NoCopyRightNoticeIsGiven" },
        { ::FrogmanEngineHeaderToolError::_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM, "_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM" },
        { ::FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax, "_InputError_IncorrectCppSyntax" },
        { ::FrogmanEngineHeaderToolError::_InputError_ParsingFailure, "_InputError_ParsingFailure" }
    });
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

# Research Papers
- The Frogman Engine Runtime Reflection System: https://1drv.ms/w/c/1b09399108712a7a/IQBmG2Q_gzsIQ7W7kQiyZHB9AZ7pBBd6sCWoF7OvIwkwVUo
