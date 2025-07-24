#include <Magick++.h>

namespace cppwal {
class ImageProcessing {
  std::vector<std::string> paths;
  std::vector<Magick::Color> colors;
  ImageProcessing(std::vector<std::string> paths);
  std::vector<Magick::Color> get_colors(); // just a getter, does nothing else
  void extract_colors();                   // calls the process function and updates the this->colors
};

std::vector<Magick::Geometry> calculate_geometry(Magick::Image& image);
std::map<Magick::Color, size_t> process_image(Magick::Image& image, Magick::Geometry& geometry);
std::string colorToHex(const Magick::Color& color);


} // namespace cppwal

/*
 - you have to find the fullpath first
 - a function to divide the image into 4 parts and process them separately
 -
 */
