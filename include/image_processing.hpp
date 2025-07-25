#pragma once

#include <Magick++.h>
#include <filesystem>

namespace cppwal {
class ImageProcessing {
  // operators
  ImageProcessing& operator=(ImageProcessing&);

  public:
  std::vector<std::filesystem::path> paths;
  std::vector<std::pair<Magick::Color, size_t>> colors;
  ImageProcessing(std::vector<std::filesystem::path> paths);
  std::vector<std::pair<Magick::Color, size_t>> get_colors();
  void extract_colors();
};

// functions that don't act on object
std::vector<Magick::Geometry> calculate_geometry(Magick::Image& image);
void process_image(Magick::Image image,
                   std::map<Magick::Color, size_t> &backparameter);
std::string colorToHex(const Magick::Color color);
void divide_image_and_process(
    std::pair<Magick::Image, std::vector<Magick::Geometry>> image_geometry,
    std::map<Magick::Color, size_t> &backparameter);

} // namespace cppwal

/*
 - you have to find the fullpath first
 - a function to divide the image into 4 parts and process them separately
 -
 */
