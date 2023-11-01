#!/bin/sh

sudo sh -c 'cat > /etc/udev/rules.d/49-stm32.rules' << EOF
# 0483:5740 - STM32F4 (USB Serial Mode)
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", ENV{MTP_NO_PROBE}="1"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", MODE="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", MODE="0666"

# 0483:df11 - STM32F4 Black Pill (DFU mode)
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="df11", MODE="0666"
EOF

sudo udevadm control --reload-rules
