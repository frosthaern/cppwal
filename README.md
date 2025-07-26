# cppwal
A high-performance C++ engine for generating color palettes from images, inspired by `pywal`.

**cppwal** is a blazing-fast C++ reimplementation of the popular [pywal](https://github.com/dylanaraps/pywal) utility. It leverages modern C++23 features and the powerful ImageMagick library to efficiently extract dominant colors from your favorite wallpapers and images. By processing multiple images in parallel, `cppwal` offers a lightweight and speedy solution for developers and themers who need to generate color schemes on the fly.

## Build Instructions

### Prerequisites
- C++23 compatible compiler (clang++)
- [ImageMagick++](https://imagemagick.org/)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- CMake >= 4.0

### With Makefile (Recommended)
You can use the provided Makefile for convenience:
```sh
make build    # Configure and build the project
make clean    # Remove all build artifacts
```
This will create the `build/` directory and place the `cppwal` executable inside it.

### Manual CMake Build
If you prefer, you can run the CMake steps manually:
```sh
mkdir -p build
cd build
cmake ..
make
```

Both methods produce the same result: the `cppwal` executable in the `build/` directory.

## Upcoming Features

- Support for exporting color palettes to various formats, including JSON, CSS, and key-value pairs.
- User-defined output templates for custom-formatted color schemes.
