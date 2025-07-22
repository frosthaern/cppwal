#include "Magick++/Color.h"
#include "Magick++/Include.h"
#include <Magick++.h>
#include <iomanip>
#include <json/json.h>
#include <map>
#include <print>
#include <iostream>
#include <sstream>

namespace Magick {
std::string colorToHex(const Magick::Color &color) {
  auto ss = std::stringstream();
  unsigned int r = color.quantumRed() * 255 / QuantumRange;
  unsigned int g = color.quantumGreen() * 255 / QuantumRange;
  unsigned int b = color.quantumBlue() * 255 / QuantumRange;
  ss << '#';
  ss << std::hex << std::setw(2) << std::setfill('0') << r;
  ss << std::hex << std::setw(2) << std::setfill('0') << g;
  ss << std::hex << std::setw(2) << std::setfill('0') << b;
  return ss.str();
}
}; // namespace Magick

int main(int argc, char **argv) {
  Magick::InitializeMagick(*argv);
  auto image = Magick::Image();
  for (int i = 1; i < argc; ++i) {
    try {
      image.read(argv[i]);
      image.quantizeColorSpace(Magick::RGBColorspace);
      image.quantizeColors(16);
      image.quantize();
      auto hist_map = std::map<Magick::Color, size_t>();
      Magick::colorHistogram(&hist_map, image);
      auto colors_hex = std::vector<std::pair<Magick::Color, size_t>>();
      for (const auto &[key, value] : hist_map) {
        colors_hex.push_back(std::make_pair(key, value));
      }
      std::sort(
          colors_hex.begin(), colors_hex.end(),
          [](auto pair1, auto pair2) { return pair1.second > pair2.second; });
      auto j = Json::Value();
      for (const auto &[idx, value] : std::views::enumerate(colors_hex)) {
	j[std::format("color{}", idx)] = Magick::colorToHex(value.first);
      }
      std::cout << j << std::endl;
    } catch (Magick::Exception &e) {
      std::print("{}", e.what());
    }
  }
}
