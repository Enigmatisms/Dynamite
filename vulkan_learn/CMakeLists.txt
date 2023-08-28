cmake_minimum_required(VERSION 3.1.0)
project(Task)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})
IF(CMAKE_BUILD_TYPE EQUAL Release)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -march=native")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
ELSE()
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check C++17 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
   add_definitions(-DCOMPILEDWITHC17)
   message(STATUS "Using flag -std=c++17.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()

include(/usr/lib/x86_64-linux-gnu/cmake/glfw3/glfw3Targets.cmake)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Boost REQUIRED COMPONENTS
  filesystem iostreams
)
if(NOT Boost_FOUND)
   message(FATAL_ERROR "Boost not found.")
endif()

# find_package(CUDA REQUIRED)
# set(
#     CUDA_NVCC_FLAGS
#     ${CUDA_NVCC_FLAGS}
#     -O3 --use_fast_math
#     -gencode=arch=compute_86,code=sm_86
#     -lcudadevrt -lcudart
#     -std=c++14
#     --expt-relaxed-constexpr
#     -Xcudafe --diag_suppress=esa_on_defaulted_function_ignored
# )

add_compile_options(-Wno-deprecated-declarations -mavx2 -mfma)

include_directories(
    .
    ./include
    /usr/local/include/
)

link_directories(/usr/local/lib)

aux_source_directory(
    ./src SRC
)

add_executable(hello ./hello_vuld.cc
    ${SRC}
)
target_link_libraries(hello ${OPENGL_gl_LIBRARY} ${GLFW3_LIBRARY} glfw vulkan)
# target_link_libraries(hello
#     stdc++fs
# )


# cuda_add_executable(cuda_test test_cuda.cu)
# set_property(TARGET cuda_test PROPERTY CXX_STANDARD 14)