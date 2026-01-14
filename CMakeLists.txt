cmake_minimum_required(VERSION 3.15)

# Do not allow building in root
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Do not build in the root. Create a bin directory and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif ()

function (merge_static_libraries target source)
    set(TARGET_PATH $<TARGET_FILE:${target}>)
    if (MSVC)
        set(TEMP_PATH "tmplib")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND mv ${TARGET_PATH} ${TEMP_PATH}
            COMMAND lib "/out:${TARGET_PATH}" ${TEMP_PATH} ${source}
            COMMAND rm ${TEMP_PATH}
            COMMENT "Relinking ${target} with ${source}")
    else ()
        set(TEMP_PATH "tmpobj")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND mkdir -p ${TEMP_PATH}
            COMMAND ${CMAKE_AR} x ${TARGET_PATH} --output ${TEMP_PATH}
            COMMAND ${CMAKE_AR} x ${source} --output ${TEMP_PATH}
            COMMAND ${CMAKE_AR} qc ${TARGET_PATH} "${TEMP_PATH}/*.o"
            COMMAND rm -rf ${TEMP_PATH}
            COMMENT "Relinking ${target} with ${source}")
    endif ()
endfunction ()

# Options
option(ENABLE_LIBPNG        "Embed libpng into the library." ON)
option(ENABLE_TESTS         "Build the unit tests for the library." ON)

# Library
project(sawyer CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SAWYER_SOURCES "src/*.cpp")
file(GLOB_RECURSE SAWYER_HEADERS "src/*.h" "src/*.hpp")
add_library(sawyer STATIC ${SAWYER_SOURCES})

if (${VCPKG_TARGET_TRIPLET} MATCHES "-static$")
    set_property(TARGET sawyer PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

if (ENABLE_LIBPNG)
    target_compile_definitions(sawyer PRIVATE CS_ENABLE_LIBPNG)
    find_package(PNG REQUIRED)

    if (TARGET PNG::PNG)
        target_link_libraries(sawyer PRIVATE PNG::PNG)
    else ()
        target_link_libraries(sawyer PRIVATE png)
    endif ()
endif ()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++17 /utf-8 /WX")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else ()

endif ()

# set_target_properties(sawyer PROPERTIES PUBLIC_HEADER "src/SawyerStream.h;src\\thirdparty/span.hpp")

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if (CMAKE_BUILD_TYPE_UPPER MATCHES "DEBUG")
    set(TARGETS_PREFIX "debug/lib")
endif ()

# Set default install prefix to out
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/out" CACHE PATH "..." FORCE)
endif()

# Expose include directory for library consumers
target_include_directories(sawyer INTERFACE $<INSTALL_INTERFACE:include>)

install(DIRECTORY "src/" DESTINATION "include/sawyer" FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(TARGETS sawyer EXPORT sawyerConfig LIBRARY ARCHIVE DESTINATION ${TARGETS_PREFIX})
install(EXPORT sawyerConfig NAMESPACE sawyer:: DESTINATION share)

## Testing
if (ENABLE_TESTS)
    find_package(GTest REQUIRED)
    set(GTEST_LIBRARIES ${GTEST_BOTH_LIBRARIES})

    file(GLOB_RECURSE SAWYER_TEST_SOURCES "test/*.cpp")
    add_executable(tests EXCLUDE_FROM_ALL ${SAWYER_TEST_SOURCES})
    target_include_directories(tests SYSTEM PRIVATE ${GTEST_INCLUDE_DIRS})
    target_include_directories(tests SYSTEM PRIVATE "${CMAKE_INSTALL_PREFIX}/include")
    add_dependencies(tests install)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(tests ${GTEST_LIBRARIES} sawyer Threads::Threads)
    gtest_discover_tests(tests)
endif ()
