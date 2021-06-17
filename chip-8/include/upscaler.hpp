/// \file upscaler.hpp
/// \brief Upscaler to convert machine display to user-specified resolution

#ifndef CHIP_8_INCLUDE_UPSCALER_HPP_
#define CHIP_8_INCLUDE_UPSCALER_HPP_

#include "chip8machine.hpp"

namespace Emulator {

/// \class Upscaler
/// \brief Upscaler to convert machine display to user-specified resolution
class Upscaler {
 public:
  Upscaler();

  void upscale(unsigned short *, int, const Chip8Machine &) const;

  /// \var x_scale
  /// \brief Scaling factor for width of display
  const int x_scale;

  /// \var y_scale
  /// \brief Scaling factor for height of display
  const int y_scale;

 private:
  const int pixel_color;
};

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_UPSCALER_HPP_
