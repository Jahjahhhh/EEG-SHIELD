#!/bin/bash

# Use the snap if arduino-cli installed with snap on ubuntu
#/snap/arduino-cli/60/usr/bin/arduino-cli compile -b esp32:esp32:esp32 ../projet6
#/snap/arduino-cli/60/usr/bin/arduino-cli upload ../projet6 -p /dev/ttyUSB0 -b esp32:esp32:esp32
#/snap/arduino-cli/60/usr/bin/arduino-cli monitor -p /dev/ttyUSB0 --config 115200

# Use this if installed from the std repo on RPi
arduino-cli compile -b esp32:esp32:esp32 ../projet6/
arduino-cli upload ../projet6/ -p /dev/ttyUSB0 -b esp23:esp32:esp32
arduino-cli monitor -p /dev/ttyUSB0 --config 115200
