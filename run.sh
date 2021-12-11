#!/bin/sh
set -ex
sudo chown root:pi /dev/i2c-1
sudo chown root:pi /dev/gpiomem
sudo chmod g+rw /dev/gpiomem
./bin/pi-stat &
