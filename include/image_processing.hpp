#include <Magick++.h>
#include <filesystem>

namespace cppwal {
class ImageProcessing {
  // operators
  ImageProcessing& operator=(ImageProcessing&);

  // functions that act on object
  std::vector<std::filesystem::path> paths;
  std::vector<Magick::Color> colors;
  ImageProcessing(std::vector<std::filesystem::path> paths);
  std::vector<Magick::Color> get_colors(); // just a getter, does nothing else
  void extract_colors();                   // calls the process function and updates the this->colors
};

// functions that don't act on object
std::vector<Magick::Geometry> calculate_geometry(Magick::Image& image);
std::map<Magick::Color, size_t> process_image(Magick::Image& image, std::map<Magick::Color, size_t> &return_parameter);
std::string colorToHex(const Magick::Color& color);
std::map<Magick::Color, size_t>
divide_image_and_process(std::pair<Magick::Image, std::vector<Magick::Geometry>>& image_geometry);


} // namespace cppwal

/*
 - you have to find the fullpath first
 - a function to divide the image into 4 parts and process them separately
 -
 */
