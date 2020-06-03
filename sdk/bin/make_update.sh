#!/bin/sh

if [ $# -gt 0 ]; then
if [ $1 == "full" ]; then
  tar cvf fitt_firmware_full.dat boot.scr u-boot_fit.min.nand u-boot_fit.bin uImage_fit rfs_fit.ubifs mcu_fitt.txt
  exit 0
fi
fi

tar cvf fitt_firmware.dat update/
exit 0
