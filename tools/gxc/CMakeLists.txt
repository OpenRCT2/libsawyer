cmake_minimum_required(VERSION 3.15)
project(gxc CXX)

file(GLOB_RECURSE SAWYER_SOURCES "src/*.cpp")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++20 /utf-8 /Zc:char8_t- /WX")
else ()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -fno-char8_t -Wno-deprecated-declarations")
    if (EXISTS "/etc/alpine-release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
    endif()
endif ()

# libsawyer
find_package(sawyer REQUIRED PATHS ../../out/share)

# libraries
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PNG REQUIRED)

add_executable(gxc ${SAWYER_SOURCES})
if (${VCPKG_TARGET_TRIPLET} MATCHES "-static$")
    set_property(TARGET gxc PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_link_libraries(gxc PRIVATE PNG::PNG)
endif ()

target_link_libraries(gxc PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(gxc PRIVATE sawyer::sawyer)
