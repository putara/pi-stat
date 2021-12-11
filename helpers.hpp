#ifndef HELPERS_HEADER_INCLUDED
#define HELPERS_HEADER_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <linux/if_link.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

struct clock {
  static bool get_time(char* str, size_t size) noexcept {
    time_t now = ::time(nullptr);
    struct tm* tm = ::localtime(&now);
    return ::strftime(str, size, "%H:%M:%S %d/%m/%Y", tm) != 0;
  }
};

struct mem_stat {
  unsigned int total;
  unsigned int used;

  bool load() noexcept {
    FILE* fp = ::fopen("/proc/meminfo", "r");
    if (fp == nullptr) {
      return false;
    }
    char text[64];
    unsigned int total = 0, avail = 0, n = 0;
    while (n < 2 && ::fgets(text, sizeof(text), fp)) {
      if (::strncmp(text, "MemTotal:", 9) == 0) {
        total = ::atoi(text + 9);
        n++;
      } else if (::strncmp(text, "MemAvailable:", 13) == 0) {
        avail = ::atoi(text + 13);
      }
    }
    ::fclose(fp);
    if (total <= 0) {
      return false;
    }
    if (avail > total) {
      avail = total;
    }
    this->total = total;
    this->used = total - avail;
    return true;
  }

  inline float usage() const noexcept {
    return static_cast<float>(used) / total;
  }
};

struct cpu_stat {
  bool loaded;
  long value[4];

  cpu_stat() noexcept {
    this->loaded = false;
  }

  bool load() noexcept {
    this->loaded = false;
    FILE* fp = ::fopen("/proc/stat", "r");
    if (fp == nullptr) {
      return false;
    }
    char text[256];
    char* field = nullptr;
    while (::fgets(text, sizeof(text), fp)) {
      if (::strncmp(text, "cpu ", 4) == 0) {
        field = text + 4;
        break;
      }
    }
    ::fclose(fp);
    if (field == nullptr) {
      return false;
    }
    this->loaded = true;
    long* value = this->value;
    *value++ = strtol(field, &field, 10);
    *value++ = strtol(field, &field, 10);
    *value++ = strtol(field, &field, 10);
    *value++ = strtol(field, &field, 10);
    return true;
  }

  float usage(const cpu_stat& prev) const noexcept {
    if (prev.loaded == false) {
      return 1; // 100% first
    }
    long num = this->value[0] - prev.value[0] + this->value[2] - prev.value[2];
    long denom = num + this->value[3] - prev.value[3];
    if (denom > 0) {
      return static_cast<float>(num) / denom;
    }
    return 0;
  }

  static float get_temp() noexcept {
    int fd = ::open("/sys/devices/virtual/thermal/thermal_zone0/temp", O_RDONLY);
    if (fd >= 0) {
      char text[256];
      ::read(fd, text, sizeof(text) - 1);
      ::close(fd);
      text[sizeof(text) - 1] = 0;
      return ::atoi(text) * 0.001;
    }
    return 0;
  }
};

struct disk_stat {
  float used;
  float total;

  bool load(const char* path) noexcept {
    struct statvfs stat;
    if (::statvfs(path, &stat) != 0) {
      return false;
    }
    const long bused = stat.f_blocks - stat.f_bavail;
    const long total = stat.f_bsize * stat.f_blocks;
    const long used = stat.f_bsize * bused;
    if (total <= 0 || used < 0) {
      return false;
    }
    this->total = static_cast<float>(total / 1024 / 1024) / 1024;
    this->used = static_cast<float>(used / 1024 / 1024) / 1024;
    return true;
  }

  float usage() noexcept {
    return this->used / this->total;
  }
};

class ifhelper {
public:
  static const size_t MAX_HOST_LEN = NI_MAXHOST;

private:
  ifaddrs* ifaddr_;

  ifaddrs* get_host_of(const char* name, char* host, size_t hostlen, int flags) const noexcept {
    const size_t namelen = strlen(name);
    for (ifaddrs* ifa = this->ifaddr_; ifa != nullptr; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == nullptr) {
        continue;
      }
      if (ifa->ifa_addr->sa_family != AF_INET) {
        continue;
      }
      if (::strncmp(ifa->ifa_name, name, namelen) != 0) {
        continue;
      }
      int ret = ::getnameinfo(ifa->ifa_addr, sizeof(sockaddr_in), host, hostlen, nullptr, 0, flags);
      if (ret == 0) {
        return ifa;
      }
      break;
    }
    return nullptr;
  }

public:
  ifhelper() noexcept {
    if (::getifaddrs(&this->ifaddr_) != 0)  {
      this->ifaddr_ = nullptr;
    }
  }

  ~ifhelper() noexcept {
    if (this->ifaddr_ != nullptr) {
      ::freeifaddrs(this->ifaddr_);
    }
  }

  const char* get_ip_address(const char* name, char* host, size_t hostlen) const noexcept {
    ifaddrs* ifa = this->get_host_of(name, host, hostlen, NI_NUMERICHOST);
    if (ifa == nullptr) {
      return nullptr;
    }
    return ifa->ifa_name;
  }
};

#endif // HELPERS_HEADER_INCLUDED
