# boost-polygon-operations

A C++ project that demonstrates various polygon operations using the Boost Geometry library and visualizes them using SFML.

## Dependencies

- CMake (version 3.10 or higher)
- C++20 compatible compiler
- Boost libraries (with system and filesystem components)
- SFML 2.5 (with graphics, window, and system components)

## Building the Project

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Generate build files:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

## Running the Application

After building, you can run the application from the build directory:
```bash
./boost_test
```

## Project Structure

- `main.cc` - Main application entry point
- `polygon.h` - Polygon operation implementations
- `polygon_visualizer.h` - SFML-based visualization utilities
- `CMakeLists.txt` - Build configuration

## Development

This project uses modern C++ features and requires a C++20 compatible compiler. The code is organized to separate the polygon operations logic from the visualization components.

## License

[Add your license information here]
