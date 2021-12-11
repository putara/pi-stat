#ifndef LCD_HEADER_INCLUDED
#define LCD_HEADER_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "types.hpp"

class lcd {
public:
  static const int WIDTH = 128;
  static const int HEIGHT = 64;
  static const int CHAR_WIDTH = 6;
  static const int CHAR_HEIGHT = 8;

private:
  uint8_t data_[1 + (WIDTH * HEIGHT / 8)];

  result_t draw_char(int x, int y, uint8_t ch, colour colour) noexcept {
#include "font.h"
    int i, j;
    if (x + 5 < 0 || x >= WIDTH || y + 7 < 0 || y >= HEIGHT) {
      return E_INVALID_ARG;
    }
    if (ch < 0x20) {
      return S_OK;
    }
    ch -= 0x20;
    for (i = 0; i < 7; i++) {
      for (j = 0; j < 5; j++) {
        if (font[ch][i] & (1 << j)) {
          this->set_pixel(x + j, y + i, colour);
        }
      }
    }
    return S_OK;
  }

  result_t draw_string_internal(int x, int y, const char* str, int len, colour colour) noexcept {
    for (int i = 0; i < len; i++) {
      this->draw_char(x, y, str[i], colour);
      x += CHAR_WIDTH;
    }
    return S_OK;
  }

  int strlen(const char* str) noexcept {
    size_t c = ::strlen(str);
    return static_cast<int>(c <= 0xffff ? c : 0xffff);
  }

public:
  lcd() noexcept {
    this->data_[0] = 0x40;
    this->clear();
  }

  ~lcd() noexcept {
  }

  inline const void* data() const noexcept {
    return this->data_;
  }

  inline const unsigned int size() const noexcept {
    return sizeof(this->data_);
  }

  result_t clear() noexcept {
    memset(this->data_ + 1, 0, sizeof(this->data_) - 1);
    return S_OK;
  }

  result_t set_pixel(int x, int y, colour colour) noexcept {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
      return E_INVALID_ARG;
    }
    uint8_t* const ptr = this->data_ + 1 + x + (y / 8) * WIDTH;
    uint8_t pixel = 1 << (y & 7);
    switch (colour) {
      case BLACK:
        *ptr &= ~pixel;
        break;
      case WHITE:
        *ptr |= pixel;
        break;
      case INVERSE:
        *ptr ^= pixel;
        break;
    }
    return S_OK;
  }

  result_t fill_rectangle(int left, int top, int right, int bottom, colour colour) noexcept {
    for (int x = left; x < right; x++) {
      for (int y = top; y < bottom; y++) {
        this->set_pixel(x, y, colour);
      }
    }
    return S_OK;
  }

  result_t draw_rectangle(int left, int top, int right, int bottom, colour colour) noexcept {
    for (int x = left; x < right; x++) {
      this->set_pixel(x, top, colour);
      this->set_pixel(x, bottom - 1, colour);
    }
    for (int y = top + 1; y < bottom - 1; y++) {
      this->set_pixel(left, y, colour);
      this->set_pixel(right - 1, y, colour);
    }
    return S_OK;
  }

  void measure_string(const char* str, int len, int* cx, int* cy) noexcept {
    *cx = len <= 0 ? 0 : CHAR_WIDTH * len;
    *cy = CHAR_HEIGHT;
  }

  result_t draw_string(int x, int y, const char* str, int len, colour colour) noexcept {
    if (len <= 0) {
      return S_OK;
    }
    if (x >= WIDTH || y >= HEIGHT) {
      return E_INVALID_ARG;
    }
    int cx, cy;
    this->measure_string(str, len, &cx, &cy);
    if (x + (cx + CHAR_WIDTH - 1) < 0 || y + (cy - 1) < 0) {
      return E_INVALID_ARG;
    }
    return this->draw_string_internal(x, y, str, len, colour);
  }

  result_t draw_string(int left, int top, int right, int bottom, const char* str, int len, colour colour, align halign, align valign) noexcept {
    if (len <= 0) {
      return S_OK;
    }
    int cx, cy;
    this->measure_string(str, len, &cx, &cy);
    int x, y;
    switch (halign) {
      case CENTRE:
        x = left + (right - left - cx) / 2;
        break;
      case RIGHT:
        x = right - cx;
        break;
      default:
        x = left;
    }
    switch (valign) {
      case CENTRE:
        y = top + (bottom - top - cy) / 2;
        break;
      case BOTTOM:
        y = bottom - cy;
        break;
      default:
        y = top;
    }
    return this->draw_string_internal(x, y, str, len, colour);
  }

  inline result_t fill_rectangle(const rect& rect, colour colour) noexcept {
    return this->fill_rectangle(rect.left, rect.top, rect.right(), rect.bottom(), colour);
  }

  inline result_t draw_rectangle(const rect& rect, colour colour) noexcept {
    return this->draw_rectangle(rect.left, rect.top, rect.right(), rect.bottom(), colour);
  }

  inline void measure_string(const char* str, int* cx, int* cy) noexcept {
    return this->measure_string(str, this->strlen(str), cx, cy);
  }

  inline void measure_string(const char* str, int len, rect* rect) noexcept {
    this->measure_string(str, len, &rect->width, &rect->height);
  }

  inline void measure_string(const char* str, rect* rect) noexcept {
    this->measure_string(str, this->strlen(str), rect);
  }

  inline result_t draw_string(int x, int y, const char* str, colour colour) noexcept {
    return this->draw_string(x, y, str, this->strlen(str), colour);
  }

  inline result_t draw_string(int left, int top, int right, int bottom, const char* str, colour colour, align halign, align valign) noexcept {
    return this->draw_string(left, top, right, bottom, str, this->strlen(str), colour, halign, valign);
  }

  inline result_t draw_string(const rect& rect, const char* str, colour colour, align halign, align valign) noexcept {
    return this->draw_string(rect.left, rect.top, rect.right(), rect.bottom(), str, colour, halign, valign);
  }

  inline result_t draw_string(const rect& rect, const char* str, int len, colour colour, align halign, align valign) noexcept {
    return this->draw_string(rect.left, rect.top, rect.right(), rect.bottom(), str, len, colour, halign, valign);
  }
};

#endif // LCD_HEADER_INCLUDED
