#ifndef SSD1306_HEADER_INCLUDED
#define SSD1306_HEADER_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <memory.h>
#include <ctime>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "types.hpp"
#include "lcd.hpp"

enum command {
  SSD1306_SETLOWCOLUMN          = 0x00,
  SSD1306_SETHIGHCOLUMN         = 0x10,
  SSD1306_MEMORYMODE            = 0x20,
  SSD1306_SETCOLUMNADDRESS      = 0x21,
  SSD1306_SETPAGEADDRESS        = 0x22,
  SSD1306_SETSTARTLINE          = 0x40,
  SSD1306_SETCONTRAST           = 0x81,
  SSD1306_CHARGEPUMP            = 0x8D,
  SSD1306_SEGREMAP              = 0xA0,
  SSD1306_DISPLAYALLON_RESUME   = 0xA4,
  SSD1306_DISPLAYALLON          = 0xA5,
  SSD1306_NORMALDISPLAY         = 0xA6,
  SSD1306_INVERTDISPLAY         = 0xA7,
  SSD1306_SETMULTIPLEX          = 0xA8,
  SSD1306_DISPLAYOFF            = 0xAE,
  SSD1306_DISPLAYON             = 0xAF,
  SSD1306_SETDISPLAYOFFSET      = 0xD3,
  SSD1306_SETDISPLAYCLOCKDIV    = 0xD5,
  SSD1306_SETPRECHARGE          = 0xD9,
  SSD1306_SETCOMPINS            = 0xDA,
  SSD1306_SETVCOMDETECT         = 0xDB,
  SSD1306_COMSCANINC            = 0xC0,
  SSD1306_COMSCANDEC            = 0xC8,
};

struct os {
  static void msleep(unsigned int ms) noexcept {
    usleep(ms * 1000);
  }
  static void wait_until_next_tick() noexcept {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    usleep(999000 - tv.tv_usec);
  }
};

class i2c {
private:
  enum status {
    SS_NOT_READY,
    SS_I2C_OPEN,
    SS_LCD_ALIVE,
    SS_LCD_DEAD = -1,
  };

  int fd_;
  status status_;

public:
  i2c() noexcept {
    this->fd_ = -1;
    this->status_ = SS_NOT_READY;
  }

  ~i2c() noexcept {
  }

  bool is_open() const noexcept {
    return this->status_ >= SS_I2C_OPEN;
  }

  void activate() noexcept {
    this->status_ = SS_LCD_ALIVE;
  }

  result_t init(const char* bus, uint8_t address) noexcept {
    int fd = ::open(bus, O_RDWR);
    if (fd < 0) {
      return E_OPEN_BUS;
    }
    if (::ioctl(fd, I2C_SLAVE, address) < 0) {
      ::close(fd);
      return E_ACQUIRE_BUS;
    }
    this->fd_ = fd;
    this->status_ = SS_I2C_OPEN;
    return S_OK;
  }

  result_t close() noexcept {
    if (this->fd_ >= 0) {
      ::close(this->fd_);
      this->fd_ = -1;
    }
    this->status_ = SS_NOT_READY;
    return S_OK;
  }

  result_t write(const void* data, unsigned int cb) noexcept {
    if (this->is_open() == false) {
      return E_NOT_READY;
    }
    if (::write(this->fd_, data, cb) != cb) {
      this->status_ = SS_LCD_DEAD;
      return E_WRITE_BUS;
    }
    return S_OK;
  }

  template <typename T, unsigned int count>
  inline result_t write(const T (&data)[count]) noexcept {
    return this->write(data, sizeof(data));
  }
};

class ssd1306 {
private:
  i2c bus_;

  result_t init_lcd() noexcept {
    result_t ret = S_OK;
#define FAIL_BAIL if (ret != S_OK) return ret; ret
    FAIL_BAIL = this->turn_off();
    FAIL_BAIL = this->command(SSD1306_SETMULTIPLEX);        // Set mux ratio
    FAIL_BAIL = this->command(lcd::HEIGHT - 1);             // height
    FAIL_BAIL = this->command(SSD1306_SETDISPLAYOFFSET);    // Set display offset
    FAIL_BAIL = this->command(0x00);                        // no vertical shift
    FAIL_BAIL = this->command(SSD1306_SETSTARTLINE | 0x0);  // Set display start line to 0
    FAIL_BAIL = this->command(SSD1306_SEGREMAP | 0x1);      // Set segment re-map, column address 127 is mapped to SEG0
    FAIL_BAIL = this->command(SSD1306_COMSCANDEC);          // Set COM output scan direction/remapped mode, scan from COM7 to COM0
    FAIL_BAIL = this->command(SSD1306_DISPLAYALLON_RESUME); // Display RAM data
    FAIL_BAIL = this->command(SSD1306_SETDISPLAYCLOCKDIV);  // Set display clock
    FAIL_BAIL = this->command(0x80);                        // max freq, no divide ratio
    FAIL_BAIL = this->command(SSD1306_CHARGEPUMP);          // Charge pump setting
    FAIL_BAIL = this->command(0x14);                        // enable charge pump
    FAIL_BAIL = this->command(SSD1306_MEMORYMODE);          // Set addressing mode
    FAIL_BAIL = this->command(0x20);                        // value = 0 act like ks0108
    FAIL_BAIL = this->turn_on();
    FAIL_BAIL = this->command(SSD1306_SETCOLUMNADDRESS);    // Set column address
    FAIL_BAIL = this->command(0x00);                        // start address
    FAIL_BAIL = this->command(0x7F);                        // end address
    FAIL_BAIL = this->command(SSD1306_SETPAGEADDRESS);      // Set page address
    FAIL_BAIL = this->command(0x00);                        // start address
    FAIL_BAIL = this->command(0x07);                        // end address
    if (lcd::HEIGHT == 64) {
      FAIL_BAIL = this->command(SSD1306_SETCOMPINS);        // Set COM pins configuration
      FAIL_BAIL = this->command(0x12);                      // alt COM pin configuration, disable COM L/R remap
      FAIL_BAIL = this->contrast(0xCF);                     // Set contrast control
    } else if (lcd::HEIGHT == 32) {
      FAIL_BAIL = this->command(SSD1306_SETCOMPINS);        // Set COM pins configuration
      FAIL_BAIL = this->command(0x02);
      FAIL_BAIL = this->contrast(0x8F);                     // Set contrast control
    } else if (lcd::HEIGHT == 16) {
      FAIL_BAIL = this->command(SSD1306_SETCOMPINS);        // Set COM pins configuration
      FAIL_BAIL = this->command(0x02);
      FAIL_BAIL = this->contrast(0xAF);                     // Set contrast control
    }
    FAIL_BAIL = this->mode(false);                          // Set non-inverting display mode
    FAIL_BAIL = S_OK;
#undef FAIL_BAIL

    this->bus_.activate();
    return ret;
  }

  inline ssd1306() noexcept {
  }

  inline ~ssd1306() noexcept {
  }

  inline void* operator new(size_t cb) noexcept {
    return ::calloc(1, cb);
  }

  inline void operator delete(void* p) noexcept {
    ::free(p);
  }

public:
  static const char* result_to_string(result_t ret) noexcept {
    switch (ret) {
      case S_OK:
        return "No error";
      case E_NO_MEMORY:
        return "Out of memory";
      case E_NOT_READY:
        return "Not ready";
      case E_OPEN_BUS:
        return "Error opening bus";
      case E_ACQUIRE_BUS:
        return "Error acquiring bus";
      case E_WRITE_BUS:
        return "Error writing bus";
      case E_INVALID_ARG:
        return "Invalid argument";
    }
    return "?";
  }

  static result_t init(const char* bus, uint8_t address, ssd1306** out) noexcept {
    *out = nullptr;
    ssd1306* that = new ssd1306();
    if (that == nullptr) {
      return E_NO_MEMORY;
    }
    result_t ret = that->bus_.init(bus, address);
    if (ret == S_OK) {
      ret = that->init_lcd();
    }
    if (ret == S_OK) {
      *out = that;
      return ret;
    }
    that->exit();
    return ret;
  }

  result_t exit() noexcept {
    this->refresh(lcd());
    this->turn_off();
    this->bus_.close();
    delete this;
    return S_OK;
  }

  result_t command(uint8_t command) noexcept {
    const uint8_t data[2] = { 0x00, command };
    return this->bus_.write(data);
  }

  result_t turn_off() noexcept {
    result_t ret = this->command(SSD1306_DISPLAYOFF);
    if (ret == S_OK) {
      os::msleep(10);
    }
    return ret;
  }

  result_t turn_on() noexcept {
    result_t ret = this->command(SSD1306_DISPLAYON);
    if (ret == S_OK) {
      os::msleep(10);
    }
    return ret;
  }

  result_t contrast(uint8_t val) noexcept {
    result_t ret = S_OK;
#define FAIL_BAIL if (ret != S_OK) return ret; ret
    FAIL_BAIL = this->command(SSD1306_SETCONTRAST);
    FAIL_BAIL = this->command(val);
    FAIL_BAIL = S_OK;
#undef FAIL_BAIL
    return ret;
  }

  result_t mode(bool invert) noexcept {
    if (invert) {
      return this->command(SSD1306_INVERTDISPLAY);
    } else {
      return this->command(SSD1306_NORMALDISPLAY);
    }
  }

  result_t refresh(const lcd& lcd) noexcept {
    result_t ret = S_OK;
#define FAIL_BAIL if (ret != S_OK) return ret; ret
    FAIL_BAIL = this->command(SSD1306_SETLOWCOLUMN  | 0x0);  // low col = 0
    FAIL_BAIL = this->command(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
    FAIL_BAIL = this->command(SSD1306_SETSTARTLINE  | 0x0);  // line #0
    FAIL_BAIL = this->bus_.write(lcd.data(), lcd.size());
    FAIL_BAIL = S_OK;
#undef FAIL_BAIL
    if (ret == S_OK) {
      os::msleep(1);
    }
    return ret;
  }
};

#endif // SSD1306_HEADER_INCLUDED
