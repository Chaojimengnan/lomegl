cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------------------------------
# Start lomegl project
# ---------------------------------------------------------------------------------------
set(VCPKG_MANIFEST_DIR ${CMAKE_SOURCE_DIR}/vcpkg)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# lomegl require at least C++20
if(NOT CMAKE_CXX_STANDARD)
    message(STATUS "CMAKE_CXX_STANDARD is not set, automatically set it to C++20")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "lomegl require at least C++20!")
endif()

# ---------------------------------------------------------------------------------------
# Set Build options
# ---------------------------------------------------------------------------------------
option(LOMEGL_USE_GLFW "Enable suporrt library for glfw" OFF)
option(LOMEGL_BUILD_TEST "Build test example(for dev purpose only)" ON)

if (LOMEGL_BUILD_TEST)
    set(LOMEGL_USE_GLFW ON)
    list(APPEND VCPKG_MANIFEST_FEATURES "glfw")
endif()

# ---------------------------------------------------------------------------------------
# Start to build lomegl library
# ---------------------------------------------------------------------------------------
project(lomegl LANGUAGES CXX)
include(GNUInstallDirs)

if (LOMEGL_USE_GLFW)
    find_package(glfw3 CONFIG REQUIRED)
endif()

find_package(glad CONFIG REQUIRED)
find_package(lotools CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(LOMEGL_SRCS
    src/gl_base.cpp
    src/gl_exception.cpp
    src/gl_object.cpp
    src/gl_shader.cpp
    src/gl_texture.cpp
    src/gl_utility.cpp
    src/gl_vertex.cpp
    src/gl_world.cpp
)

message(STATUS "Build lomegl")
add_library(lomegl STATIC ${LOMEGL_SRCS})
add_library(lomegl::lomegl ALIAS lomegl)
target_include_directories(lomegl
PUBLIC 
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(lomegl PUBLIC glad::glad lotools::lotools glm::glm)

# ---------------------------------------------------------------------------------------
# Start to build lomegl-glfw library(if has)
# ---------------------------------------------------------------------------------------
if (LOMEGL_USE_GLFW)

    set(LOMEGL_GLFW_SRCS
    src/loglfw/glfw_utility.cpp
    )

    # Build lomegl-glfw
    message(STATUS "Build lomegl-glfw")
    add_library(lomegl-glfw STATIC ${LOMEGL_GLFW_SRCS})
    add_library(lomegl::lomegl-glfw ALIAS lomegl-glfw)
    target_include_directories(lomegl-glfw
    PUBLIC 
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
    target_link_libraries(lomegl-glfw PUBLIC lomegl::lomegl glfw)
endif()

# ---------------------------------------------------------------------------------------
# Start to build lomegl-test(for dev purpose only)
# ---------------------------------------------------------------------------------------

if (LOMEGL_BUILD_TEST)
    add_executable(lomegl-test test/main.cpp)
    target_include_directories(lomegl-test PRIVATE test)
    target_link_libraries(lomegl-test PRIVATE lomegl::lomegl-glfw)
endif()

# ---------------------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------------------
message(STATUS "Generating install")

list(APPEND lib_name "lomegl")
if (LOMEGL_USE_GLFW)
    list(APPEND lib_name "lomegl-glfw")
endif()

install(
    TARGETS ${lib_name} EXPORT lomegl-config
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT lomegl-config
    NAMESPACE lomegl::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/lomegl
    FILE lomegl-config-targets.cmake
)

include(CMakePackageConfigHelpers)
configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/lomegl-config.cmake.in" 
        "${CMAKE_CURRENT_BINARY_DIR}/lomegl-config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/lomegl)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lomegl-config.cmake" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/lomegl)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/lomegl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp*" PATTERN "*.inl*" PATTERN "*.h*"
)
