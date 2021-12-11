#include "fauxhelp.hpp"
#include "../render.hpp"
#include "../config.hpp"

#include <cstdio>

#define SVG_PREFER_PATH

int main() noexcept {
  config config;
  if (config.load() == false) {
    return EXIT_FAILURE;
  }

  renderer canvas;
  canvas.start();
  canvas.print_title(config.title);
  canvas.print_time();
  canvas.print_ip(config.ifnames);
  canvas.print_cpu();
  canvas.print_mem();
  canvas.print_disk(config.mountpoint);
  auto lcd = canvas.end();
  auto data = static_cast<const uint8_t*>(lcd.data());
  auto fp = ::fopen("lcd.svg", "w");
  if (fp == nullptr) {
    return EXIT_FAILURE;
  }
  ::fprintf(fp,
    "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 %d %d\">"
    "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"seagreen\"/>"
    "<rect x=\"3\" y=\"3\" width=\"%d\" height=\"%d\" fill=\"black\"/>",
    4 * lcd::WIDTH + 7, 4 * lcd::HEIGHT + 7,
    4 * lcd::WIDTH + 7, 4 * lcd::HEIGHT + 7,
    4 * lcd::WIDTH + 1, 4 * lcd::HEIGHT + 1);

#ifdef SVG_PREFER_PATH
  ::fprintf(fp, "<path d=\"");
#endif

  for (int y = 0; y < lcd::HEIGHT; y++) {
    for (int x = 0; x < lcd::WIDTH; x++) {
      auto ptr = data + 1 + x + (y / 8) * lcd::WIDTH;
      if (*ptr & 1 << (y & 7)) {
#ifdef SVG_PREFER_PATH
        ::fprintf(fp,
          "M%d,%d"
          "L%d,%d"
          "L%d,%d"
          "L%d,%d"
          "L%d,%d"
          "z",
          (x + 1) * 4, (y + 1) * 4,
          (x + 1) * 4, (y + 2) * 4 - 1,
          (x + 2) * 4 - 1, (y + 2) * 4 - 1,
          (x + 2) * 4 - 1, (y + 1) * 4,
          (x + 1) * 4, (y + 1) * 4);
#else
        ::fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"3\" height=\"3\" fill=\"white\"/>", 1 + x * 4, 1 + y * 4);
#endif
      }
    }
  }

#ifdef SVG_PREFER_PATH
  ::fprintf(fp, "\" fill=\"white\"/>");
#endif

  ::fprintf(fp, "</svg>");

  return EXIT_SUCCESS;
}
