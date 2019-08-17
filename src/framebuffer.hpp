#pragma once

#include <stdint.h>
#include <string>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

class Framebuffer {
 public:
  static constexpr size_t colorChannels = 3;

  Framebuffer(size_t width, size_t height)
      : width_(width),
        height_(height),
        data_((unsigned char *)malloc(width * height * colorChannels)) {}

  ~Framebuffer() { free(data_); }

  // Set pixel [row, col] to color (r, g, b)
  void set_pixel(size_t row, size_t column, uint8_t r, uint8_t g, uint8_t b) {
    const size_t idx = colorChannels * (row * width_ + column);
    data_[idx + 0] = r;
    data_[idx + 1] = g;
    data_[idx + 2] = b;
  }

  // Save framebuffer to .PNG file
  int save(const char *file_name) {
    return stbi_write_png(file_name, width_, height_, 3, data_, 0);
  }

  size_t width() { return width_; }
  size_t height() { return height_; }

 private:
  unsigned char *data_;
  size_t width_;
  size_t height_;
};