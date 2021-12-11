#include "ssd1306.hpp"
#include "helpers.hpp"
#include "render.hpp"
#include "config.hpp"

#include <signal.h>

volatile sig_atomic_t done = 0;
struct sigaction action;

void sig_handler(int signo) noexcept {
  done = 1;
  // printf("signal: %d\n", signo);
}

int main() noexcept {
  printf("pid: %d\n", getpid());
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = sig_handler;
  if (sigaction(SIGINT, &action, nullptr) == -1) {
    perror("Error: cannot handle SIGINT");
  }
  if (sigaction(SIGTERM, &action, nullptr) == -1) {
    perror("Error: cannot handle SIGTERM");
  }

  config config;
  if (config.load() == false) {
    return EXIT_FAILURE;
  }

  ssd1306* dev;
  auto ret = ssd1306::init(config.i2c_dev, config.i2c_addr, &dev);
  if (ret == S_OK) {
    printf("ready\n");
    renderer canvas;
    while (!done) {
      canvas.start();
      canvas.print_title(config.title);
      canvas.print_time();
      canvas.print_ip(config.ifnames);
      canvas.print_cpu();
      canvas.print_mem();
      canvas.print_disk(config.mountpoint);
      auto lcd = canvas.end();
      dev->refresh(lcd);
      os::wait_until_next_tick();
    }
    printf("\nexiting\n");
    dev->exit();
  }

  return EXIT_SUCCESS;
}
