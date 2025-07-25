#include "image_processing.hpp"
#include <Magick++.h>
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <print>
#include <thread>

namespace cppwal {
ImageProcessing::ImageProcessing(std::vector<std::filesystem::path> paths)
    : paths(paths), colors() {};

std::vector<Magick::Color> ImageProcessing::get_colors() {
  return this->colors;
}

void ImageProcessing::extract_colors() {
  auto images =
      std::vector<std::pair<Magick::Image, std::vector<Magick::Geometry>>>();
  auto image_maps = std::vector<std::map<Magick::Color, size_t>>(
      images.size(), std::map<Magick::Color, size_t>());

  std::transform(this->paths.begin(), this->paths.end(),
                 std::back_inserter(images), [](auto file_path) {
                   auto image = Magick::Image(file_path.string());
                   image.read(file_path.string());
                   auto geometries = calculate_geometry(image);
                   return std::make_pair(image, geometries);
                 });

  auto threads = std::vector<std::thread>();
  for (int i = 0; i < images.size(); ++i) {
    threads.push_back(
        std::thread(divide_image_and_process, images[i], image_maps[i]));
  }
  std::for_each(threads.begin(), threads.end(),
                [](std::thread& thread) { thread.join(); });
  /*
   * TODO: combine all those hashmap values into one value and put in it a
   * vector and sort it and save it in the class variable
   */
}

// i have to check if this one works too
void divide_image_and_process(
    std::pair<Magick::Image, std::vector<Magick::Geometry>>& image_geometry,
    std::map<Magick::Color, size_t>& backparameter) {
  auto total_map = std::map<Magick::Color, size_t>();
  auto m1 = std::map<Magick::Color, size_t>();
  auto m2 = std::map<Magick::Color, size_t>();
  auto m3 = std::map<Magick::Color, size_t>();
  auto m4 = std::map<Magick::Color, size_t>();
  Magick::Image top_right(image_geometry.first);
  top_right.crop(image_geometry.second[0]);
  Magick::Image top_left(image_geometry.first);
  top_left.crop(image_geometry.second[1]);
  Magick::Image bottom_left(image_geometry.first);
  bottom_left.crop(image_geometry.second[2]);
  Magick::Image bottom_right(image_geometry.first);
  bottom_right.crop(image_geometry.second[3]);
  std::thread t1(cppwal::process_image(top_right, m1));
  std::thread t2(cppwal::process_image(top_left, m2));
  std::thread t3(cppwal::process_image(bottom_left, m3));
  std::thread t4(cppwal::process_image(bottom_right, m4));
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  for (const auto& [key, value] : m1) total_map[key] += value;
  for (const auto& [key, value] : m2) total_map[key] += value;
  for (const auto& [key, value] : m3) total_map[key] += value;
  for (const auto& [key, value] : m4) total_map[key] += value;
  backparameter = total_map;
}

std::vector<Magick::Geometry> calculate_geometry(Magick::Image& image) {
  auto geometries = std::vector<Magick::Geometry>();
  size_t cols = image.columns();
  size_t rows = image.rows();
  size_t half_cols = cols / 2; // which is also not of cols in one quadrant
  size_t half_rows = rows / 2; // which is also not of rows in one quadrant
  geometries.push_back(Magick::Geometry(half_cols, half_cols, 0));
  geometries.push_back(Magick::Geometry(half_cols, 0, 0));
  geometries.push_back(Magick::Geometry(half_cols, 0, half_rows));
  geometries.push_back(Magick::Geometry(half_cols, half_cols, half_rows));
  return geometries;
}

// check if this works
void process_image(Magick::Image image,
                   std::map<Magick::Color, size_t>& backparameter) {
  image.quantizeColorSpace(Magick::RGBColorspace);
  image.quantizeColors(16);
  image.quantize();
  auto hist_map = std::map<Magick::Color, size_t>();
  Magick::colorHistogram(&hist_map, image);
  backparameter = hist_map;
}

std::string colorToHex(const Magick::Color& color) {
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
