project(scriptbots)
cmake_minimum_required(VERSION 2.8)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLUT using pkg-config (modern approach for Linux)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLUT REQUIRED glut)

# Fallback for Windows or if pkg-config fails
if (WIN32 AND NOT GLUT_FOUND)
    find_library(GLUT_LIBRARY NAMES glut PATHS ${CMAKE_CURRENT_SOURCE_DIR})
    find_library(GLUT32_LIBRARY NAMES glut32 PATHS ${CMAKE_CURRENT_SOURCE_DIR})
    SET (LOCAL_GLUT32 1)
endif()

find_package(OpenMP)

if (OPENMP_FOUND)
    SET (HAVE_OPENMP 1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set( SB_SRCS
    View.cpp
    GLView.cpp
    main.cpp
    DWRAONBrain.cpp
    MLPBrain.cpp
    AssemblyBrain.cpp
    Agent.cpp
    World.cpp
    vmath.cpp )

add_executable(scriptbots  ${SB_SRCS})

# Link libraries
if (WIN32 AND NOT GLUT_FOUND)
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLUT32_LIBRARY})
else()
    target_link_libraries(scriptbots ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
endif()

# Add compiler flags from pkg-config
target_compile_options(scriptbots PRIVATE ${GLUT_CFLAGS_OTHER})
