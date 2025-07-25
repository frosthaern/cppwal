#include <Magick++.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <json/json.h>
#include <print>
#include <ranges>
#include "image_processing.hpp"

int main(int argc, char** argv) {
  Magick::InitializeMagick(*argv);
  auto paths = std::vector<std::filesystem::path>();
  for (int i = 1; i < argc; ++i)
    paths.push_back(std::filesystem::absolute(argv[i]));
  auto image_processor = cppwal::ImageProcessing(paths);
  image_processor.extract_colors();
  auto colors = image_processor.get_colors();

  std::for_each_n(colors.begin(), 10, [](auto color) {
    std::println("Red: {}\nGreen: {}\nBlue: {}\nPixels: {}\n", color.first.quantumRed(), color.first.quantumGreen(), color.first.quantumBlue(), color.second);
  });
}

/*
 - what main.cpp is suppose to do
 - call the image processing cstor and initialize
 - process the images but in a parallel way
 - so the image_processing class is supposed to do that in parallel
 - then collect all the colors and sort it and keep it
 - then write a function to get the copy of vector to pass it to other classes
that implement formatted_write_to_file
-
 */
