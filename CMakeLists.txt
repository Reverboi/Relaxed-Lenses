cmake_minimum_required(VERSION 3.0)

project(Relaxed-Lenses)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add include directory
include_directories(include)

# Generate the executable
add_executable(${PROJECT_NAME} ${SOURCES})
