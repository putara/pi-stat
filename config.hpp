#ifndef CONFIG_HEADER_INCLUDED
#define CONFIG_HEADER_INCLUDED

struct config {
  const char* i2c_dev;
  uint8_t i2c_addr;
  const char* ifnames;
  const char* mountpoint;
  const char* title;

  bool load() noexcept {
    // TODO: load config file
    this->i2c_dev = "/dev/i2c-1";
    this->i2c_addr = 0x3c;
    this->ifnames = "eth\0wlan\0";
    this->mountpoint = "/";
    this->title = "Raspberry Pi";
    return true;
  }
};

#endif // CONFIG_HEADER_INCLUDED
