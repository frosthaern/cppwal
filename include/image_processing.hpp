#pragma once

#include <Magick++.h>
#include <filesystem>

namespace cppwal {
class ImageProcessing {
  public:
  ImageProcessing& operator=(ImageProcessing&);
  std::vector<std::filesystem::path> paths;
  ImageProcessing(std::vector<std::filesystem::path> paths);;
  std::vector<std::pair<Magick::Color, size_t>> extract_colors();
};
const std::map<Magick::Color, size_t> process_image(Magick::Image image);
std::string colorToHex(const Magick::Color color);

} // namespace cppwal
