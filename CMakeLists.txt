cmake_minimum_required(VERSION 3.10)
project(boost_test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# Add executable
add_executable(${PROJECT_NAME} main.cc)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES}
    sfml-graphics
    sfml-window
    sfml-system
)
