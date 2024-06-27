#!/bin/sh

sudo sh -c 'cat > /etc/udev/rules.d/48-avr.rules' << EOF
# 2a03:0043 dog hunter AG Arduino Uno Rev3

ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="0043", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="0043", ENV{MTP_NO_PROBE}="1"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="0043", MODE="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="0043", MODE="0666"

# 2341:0042 Arduino SA Mega 2560 R3 (CDC ACM)

ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0042", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0042", ENV{MTP_NO_PROBE}="1"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0042", MODE="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0042", MODE="0666"

EOF

sudo udevadm control --reload-rules
