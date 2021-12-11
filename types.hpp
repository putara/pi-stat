#ifndef TYPES_HEADER_INCLUDED
#define TYPES_HEADER_INCLUDED

enum colour {
  BLACK,
  WHITE,
  INVERSE,
};

enum align {
  LEFT,
  RIGHT,
  CENTRE,
  TOP = LEFT,
  BOTTOM = RIGHT,
};

enum result_t {
  S_OK,
  E_NO_MEMORY,
  E_NOT_READY,
  E_OPEN_BUS,
  E_ACQUIRE_BUS,
  E_WRITE_BUS,
  E_INVALID_ARG,
};

struct rect {
  int left;
  int top;
  int width;
  int height;

  rect() noexcept {
  }

  rect(int left, int top, int width, int height) noexcept {
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
    this->normalise();
  }

  int right() const noexcept {
    return this->left + this->width;
  }

  int bottom() const noexcept {
    return this->top + this->height;
  }

  void right(int right) noexcept {
    this->width = right - this->left;
    this->normalise();
  }

  void bottom(int bottom) noexcept {
    this->height = bottom - this->top;
    this->normalise();
  }

  void normalise() noexcept {
    if (this->width < 0) {
      this->width = 0;
    }
    if (this->height < 0) {
      this->height = 0;
    }
  }
};

#endif // TYPES_HEADER_INCLUDED
