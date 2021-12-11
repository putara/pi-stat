#ifndef HELPERS_HEADER_INCLUDED
#define HELPERS_HEADER_INCLUDED

#include <cstring>

struct clock {
  static bool get_time(char* str, size_t size) noexcept {
    if (size < 20) {
      return false;
    }
    strcpy(str, "12:34:56 07/08/2021");
    return true;
  }
};

struct mem_stat {
  unsigned int total;
  unsigned int used;

  bool load() noexcept {
    this->total = 3628800;
    this->used = this->total - 1594323;
    return true;
  }

  inline float usage() const noexcept {
    return static_cast<float>(used) / total;
  }
};

struct cpu_stat {
  bool load() noexcept {
    return true;
  }

  float usage(const cpu_stat& prev) const noexcept {
    return 0.15625;
  }

  static float get_temp() noexcept {
    return 32.768;
  }
};

struct disk_stat {
  float used;
  float total;

  bool load(const char* path) noexcept {
    this->total = 29.3141592;
    this->used = 27.1828182;
    return true;
  }

  float usage() noexcept {
    return this->used / this->total;
  }
};

struct ifhelper {
  static const size_t MAX_HOST_LEN = sizeof("xxx.xxx.xxx.xxx");

  const char* get_ip_address(const char* name, char* host, size_t hostlen) const noexcept {
    strncpy(host, "192.168.1.123", hostlen - 1);
    host[hostlen - 1] = 0;
    return "eth0";
  }
};

#endif // HELPERS_HEADER_INCLUDED
