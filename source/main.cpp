#include <Magick++.h>
#include <iostream>
#include <json/json.h> 
#include <print>
#include <ranges>
#include "image_processing.hpp"


int main(int argc, char** argv) {
  Magick::InitializeMagick(*argv);
  auto image = Magick::Image();
  for (int i = 1; i < argc; ++i) {
    try {
      image.read(argv[i]);
      image.quantizeColorSpace(Magick::RGBColorspace); // there is probably a cropping part missing somewhere
      image.quantizeColors(16);
      image.quantize();
      auto hist_map = std::map<Magick::Color, size_t>();
      Magick::colorHistogram(&hist_map, image);
      auto colors_hex = std::vector<std::pair<Magick::Color, size_t>>();
      for (const auto& [key, value] : hist_map) {
        colors_hex.push_back(std::make_pair(key, value));
      }
      std::sort(colors_hex.begin(), colors_hex.end(),
                [](auto pair1, auto pair2) { return pair1.second > pair2.second; });
      auto j = Json::Value();
      for (const auto& [idx, value] : std::ranges::views::enumerate(colors_hex)) {
        j[std::format("color{}", idx)] = cppwal::colorToHex(value.first);
      }
      std::cout << j << std::endl;
    } catch (Magick::Exception& e) {
      std::print("{}", e.what());
    }
  }
}

/*
 - what main.cpp is suppose to do
 - call the image processing cstor and initialize
 - process the images but in a parallel way
 - so the image_processing class is supposed to do that in parallel
 - then collect all the colors and sort it and keep it
 - then write a function to get the copy of vector to pass it to other classes that implement formatted_write_to_file
-
 */
  
