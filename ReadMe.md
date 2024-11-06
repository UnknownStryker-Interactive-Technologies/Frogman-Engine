[![License](https://img.shields.io/badge/License-boost-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-x86_64-white.svg)](PLATFORM)

# Frogman-Engine
Frogman Engine License v2024.10.17
Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

DEFINITIONS
  FROGMAN ENGINE:
    Game development toolkit and the source code created by UNKNOWN STRYKER.

  THE SOFTWARE:
	FROGMAN ENGINE.

  THE PROVIDER:
    1. UNKNOWN STRYKER is the original author and the developer of FROGMAN ENGINE.
    2. UNKNOWN STRYKER is a nickname and his real name in Korean is 이호진. The pronunciation of the Korean name is Hojin Lee.

  THE USERS:
    Any person who Use, Reproduce, Modify, and Re-distribute FROGMAN ENGINE.

  THE APP POWERED BY FROGMAN ENGINE:
	1. Any software that has the dependancy to FROGMAN ENGINE.
    2. Any software that uses the source code of FROGMAN ENGINE.


THE TERMS AND LIABILITIES
  1. THE SOFTWARE is free to Use, Reproduce, Execute, Modify, and Re-distribute in any kind of form.
  2. The USERS are permitted to use THE SOFTWARE for any purpose, including commercial and non-commercial.
  3. Utilizing THE SOFTWARE for malicious, illegal, or unethical purposes is restricted, and this term of restriction is the exception to the First and the Second term.
  4. THE PROVIDER is Not responsible for THE USERS violating the Third term, and any damage or loss caused by THE SOFTWARE.
  5. THE USERS must retain the original copy of this license in all copies of THE SOFTWARE.
  6. THE APP POWERED BY FROGMAN ENGINE has to display a message or the official Frogman Engine logo on the screen when the app is launched.
  7. Any modifications made by THE USERS to THE SOFTWARE must be documented and plainly explained in the source code comments.
  8. THE USERS are not allowed to claim the authorship and the copyright of THE SOFTWARE.

The permission is hereby granted, in compliance with THE TERMS AND LIABILITIES of this license. 

This project is under development and may contain some bugs and issues.

# Dev Team Discord Link: https://discord.gg/jhu4vAU3Mm
Currently, Unknown Stryker(Hojin Lee) is the only active developer for this project.

# Frogman Engine Coding Conventions and Rules.
https://savory-moth-a00.notion.site/5cef12dd2c9b43588f22045405e80cff?v=86ea5413ff4f40de9204c4aa22907ffc&pvs=4

# Prerequisite: CMake 3.27.0 or latest, and LLVM Clang 10 or latest [ includes LLVM (clangcl) ].

# C++ standard version: C++ 20 or latest.

All project settings can be adjusted by modifying CMakeLists.txt.

# The current development status.
| Platform     | Architecture  | Status           |
|--------------|---------------|------------------|
| Windows 11   | X86-64        | In Development   |
| Ubuntu-Linux | X86-64        | Discontinued     |

# This project uses:
- Boost Libraries
https://github.com/boostorg/boost/releases/tag/boost-1.85.0

- Google Test
https://github.com/google/googletest/releases/tag/v1.14.0

- Google Benchmark
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
