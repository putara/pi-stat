#ifndef RENDER_HEADER_INCLUDED
#define RENDER_HEADER_INCLUDED

#ifndef HELPERS_HEADER_INCLUDED
#error helpers not included
#endif

#include "lcd.hpp"
#include <cstdio>

class renderer {
private:
  int y_;
  lcd lcd_;
  cpu_stat cpu_;

  inline void advance_y(int delta) noexcept {
    this->y_ += delta;
  }

  inline void next_line() noexcept {
    this->advance_y(lcd::CHAR_HEIGHT);
  }

  void draw_bar_chart(const char* name, float value, const char* text, bool round) noexcept {
    const int x = lcd::CHAR_WIDTH * 4;
    const int y = this->y_;
    const int height = lcd::CHAR_HEIGHT + (lcd::HEIGHT == 64 ? 2 : 1);
    this->lcd_.draw_rectangle(x, y, lcd::WIDTH, y + height + 1, WHITE);
    if (round) {
      this->lcd_.set_pixel(x, y, BLACK);
      this->lcd_.set_pixel(lcd::WIDTH - 1, y, BLACK);
      this->lcd_.set_pixel(x, y + height, BLACK);
      this->lcd_.set_pixel(lcd::WIDTH - 1, y + height, BLACK);
    }
    this->lcd_.fill_rectangle(x + 1, y + 1, x + 1 + (lcd::WIDTH - 2 - x) * value, y + height, WHITE);
    this->lcd_.draw_string(0, y, lcd::WIDTH, y + height + 2, name, INVERSE, LEFT, CENTRE);
    this->lcd_.draw_string(x + 2, y, lcd::WIDTH - 2 - lcd::CHAR_WIDTH * 3, y + height + 2, text, INVERSE, CENTRE, CENTRE);
    char percent[32];
    snprintf(percent, sizeof(percent), "%.f%%", value * 100);
    this->lcd_.draw_string(0, y, lcd::WIDTH - 2, y + height + 2, percent, INVERSE, RIGHT, CENTRE);
    this->advance_y(height + 2);
  }

  void draw_pattern(int x1, int y1, int x2, int y2) noexcept {
    for (int x = x1; x < x2; x++) {
      int s = x & 3;
      int pat = ((3 >> s) | (3 << (4 - s))) & 15;
      for (int y = y1; y < y2; y++) {
        if (pat & (1 << (y & 3))) {
          this->lcd_.set_pixel(x, y, WHITE);
        }
      }
    }
  }

public:
  renderer() noexcept {
    this->y_ = 0;
  }
  ~renderer() noexcept {
  }

  void start() noexcept {
    this->lcd_.clear();
    this->y_ = 0;
  }

  const lcd& end() noexcept {
    // char x[16];sprintf(x,"%d",this->y_);this->lcd_.draw_string(0, 0, x, WHITE);
    this->y_ = lcd::HEIGHT;
    return this->lcd_;
  }

  void print_title(const char* str) noexcept {
    int cx, cy;
    this->lcd_.measure_string(str, &cx, &cy);
    int x = (lcd::WIDTH - cx) / 2;
    this->lcd_.draw_string(x, this->y_ + 1, str, WHITE);
    this->draw_pattern(0, this->y_, x - 4, this->y_ + lcd::CHAR_HEIGHT);
    this->draw_pattern(x + cx + 3, this->y_, lcd::WIDTH, this->y_ + lcd::CHAR_HEIGHT);
    this->next_line();
    this->advance_y(2);
  }

  void print_time() noexcept {
    char text[32];
    if (clock::get_time(text, sizeof(text))) {
      this->lcd_.draw_string(0, this->y_, lcd::WIDTH, this->y_ + lcd::CHAR_HEIGHT, text, WHITE, CENTRE, CENTRE);
      this->next_line();
    }
  }

  void print_ip(const char* names) noexcept {
    char host[ifhelper::MAX_HOST_LEN];
    ifhelper helper;
    for (const char* name = names; *name != '\0'; name += strlen(name) + 1) {
      const char* ifa_name = helper.get_ip_address(name, host, sizeof(host));
      if (ifa_name == nullptr) {
        continue;
      }
      int len = ::strlen(ifa_name);
      rect rect;
      rect.left = 0;
      rect.top = this->y_;
      this->lcd_.measure_string(ifa_name, len, &rect);
      rect.width += 1;
      rect.height += 1;
      this->lcd_.fill_rectangle(rect, WHITE);
      rect.top++;
      this->lcd_.draw_string(rect, ifa_name, len, INVERSE, RIGHT, CENTRE);
      rect.left = rect.right() + 1;
      rect.right(lcd::WIDTH);
      this->lcd_.draw_string(rect, host, WHITE, CENTRE, CENTRE);
      this->next_line();
      this->advance_y(2);
      return;
    }
  }

  void print_mem() noexcept {
    mem_stat stat;
    if (stat.load()) {
      const float usage = stat.usage();
      char text[256];
      ::snprintf(text, sizeof(text), "%.1f/%.1f" "\xf0\xf1",
        static_cast<float>(stat.used / 1024) / 1024,
        static_cast<float>(stat.total / 1024) / 1024
      );
      this->draw_bar_chart("Mem", usage, text, true);
    }
  }

  void print_cpu() noexcept {
    cpu_stat cur;
    if (cur.load()) {
      const float usage = cur.usage(this->cpu_);
      const float temp = cpu_stat::get_temp();
      this->cpu_ = cur;
      char text[32];
      if (temp > 0) {
        snprintf(text, sizeof(text), "%.1f" "\xb0" "c", temp);
      } else {
        text[0] = '\0';
      }
      return this->draw_bar_chart("CPU", usage, text, true);
    }
  }

  void print_disk(const char* path) noexcept {
    disk_stat stat;
    if (stat.load(path)) {
      const float usage = stat.usage();
      char text[64];
      snprintf(text, sizeof(text), "%.f/%.f" "\xf0\xf1",
        stat.used,
        stat.total
      );
      return this->draw_bar_chart("Disk", usage, text, true);
    }
  }
};

#endif // RENDER_HEADER_INCLUDED
