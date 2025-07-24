#include "image_processing.hpp"
#include <Magick++.h>
#include <print>
#include <iomanip>

namespace cppwal {
ImageProcessing::ImageProcessing(std::vector<std::string> paths) : paths(paths), colors() {};

std::vector<Magick::Color> ImageProcessing::get_colors() {
  return this->colors;
}

void ImageProcessing::extract_colors() {
  // here you are suppose to figure out the correct path for all files
  // and read that files properly
  // after that call the process_image() in 4 threads and different geometry for all files

  
}

std::vector<Magick::Geometry> calculate_geometry(Magick::Image& image) {
  auto geometries = std::vector<Magick::Geometry>();
  // take the image height and  width and stuff and calculate geometry, go anti click wise direction like in math
  // like 1st, 2nd, 3rd, 4th quadrant geometries push it into the vector and return it
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

std::map<Magick::Color, size_t> process_image(Magick::Image image, Magick::Geometry geometry) {
  image.crop(geometry);
  image.quantizeColorSpace(Magick::RGBColorspace);
  image.quantizeColors(16);
  image.quantize();
  auto hist_map = std::map<Magick::Color, size_t>();
  Magick::colorHistogram(&hist_map, image);
  return hist_map;
}

std::string colorToHex(const Magick::Color& color) {
  auto ss = std::stringstream();

  const auto quantum = 65535;
  // technically you are supposed to use Magick::Quantum() for this but i am getting 0 so i am hard coding for now
  
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
