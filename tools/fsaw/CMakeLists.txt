cmake_minimum_required(VERSION 3.15)
project(fsaw CXX)

find_package(sawyer REQUIRED PATHS ../../out/share)

file(GLOB_RECURSE SAWYER_SOURCES "src/*.cpp")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++20 /utf-8 /Zc:char8_t- /WX")
else ()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-char8_t -Wno-deprecated-declarations")
endif ()

# libsawyer
find_package(sawyer REQUIRED PATHS ../../out/share)

# libraries
find_package(PNG REQUIRED)

add_executable(fsaw ${SAWYER_SOURCES})
if (${VCPKG_TARGET_TRIPLET} MATCHES "-static$")
    set_property(TARGET fsaw PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

target_link_libraries(fsaw sawyer::sawyer)
