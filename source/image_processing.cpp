#include "image_processing.hpp"
#include <Magick++.h>
#include <filesystem>
#include <future>
#include <iomanip>
#include "Magick++/Image.h"

namespace cppwal {
ImageProcessing::ImageProcessing(std::vector<std::filesystem::path> paths) : paths(paths) {};



std::vector<std::pair<Magick::Color, size_t>> ImageProcessing::extract_colors() {
  auto N = this->paths.size();
  auto images = std::vector<Magick::Image>(N, Magick::Image());

  for (int i = 0; i < N; ++i) images[i].read(this->paths[i]);

  auto futures = std::vector<std::future<const std::map<Magick::Color, size_t>>>();
  for (const auto& image : images) futures.push_back(std::async(std::launch::async, process_image, image));

  auto processed_image_maps = std::vector<std::map<Magick::Color, size_t>>();
  for (auto& future : futures) processed_image_maps.push_back(future.get());

  auto hm = std::map<Magick::Color, size_t>();
  for (const auto& map : processed_image_maps)
    for (const auto& [key, value] : map) hm[key] += value;

  auto processed_colors = std::vector<std::pair<Magick::Color, size_t>>();

  for (const auto& [key, value] : hm) processed_colors.push_back(std::make_pair(key, value));

  std::ranges::sort(processed_colors, [](auto& a, auto& b) { return a.second < b.second; });

  return processed_colors;
}


  
const std::map<Magick::Color, size_t> process_image(Magick::Image image) {
  image.resize(Magick::Geometry(100, 100));
  image.quantizeColorSpace(Magick::RGBColorspace);
  image.quantizeColors(8);
  image.quantize();
  auto hist_map = std::map<Magick::Color, size_t>();
  Magick::colorHistogram(&hist_map, image);
  return hist_map;
}


  
std::string colorToHex(const Magick::Color color) {
  auto ss = std::stringstream();
  const auto quantum = 65535;
  // technically you are supposed to use Magick::Quantum() for this but i am
  // getting 0 so i am hard coding for now why don't you check out the function
  // Magick::GetQuantumRange() and see it works
  unsigned int r = color.quantumRed() * 255 / quantum;
  unsigned int g = color.quantumGreen() * 255 / quantum;
  unsigned int b = color.quantumBlue() * 255 / quantum;
  ss << '#';
  ss << std::hex << std::setw(2) << std::setfill('0') << r;
  ss << std::hex << std::setw(2) << std::setfill('0') << g;
  ss << std::hex << std::setw(2) << std::setfill('0') << b;
  return ss.str();
}

} // namespace cppwal
