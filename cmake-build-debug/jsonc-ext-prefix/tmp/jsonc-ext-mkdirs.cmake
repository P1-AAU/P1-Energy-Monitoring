# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src/jsonc-ext"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src/jsonc-ext-build"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/tmp"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src/jsonc-ext-stamp"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src"
  "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src/jsonc-ext-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Christian/Documents/GitHub/P1-Energy-Monitoring/cmake-build-debug/jsonc-ext-prefix/src/jsonc-ext-stamp/${subDir}")
endforeach()
