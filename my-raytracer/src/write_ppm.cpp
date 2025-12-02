#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert(
    (num_channels == 3 || num_channels ==1 ) &&
    ".ppm only supports RGB or grayscale images");
  ////////////////////////////////////////////////////////////////////////////
  std::ofstream ofs(filename);
  if (!ofs.is_open()) {
    return false;
  }

  if (num_channels == 3) {
    ofs << "P3\n";
  } else if (num_channels == 1) {
    ofs << "P2\n";
  }
  ofs << width << " " << height << "\n255\n";

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      // write pixel value(s)
      for (int ch = 0; ch < num_channels; ch++) {
        ofs << static_cast<int>(data[num_channels * (row * width + col) + ch]) << " ";
      }
    }
    // line finished, write a newline
    ofs << "\n";
  }

  ofs.close();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
