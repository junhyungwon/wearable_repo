#!/bin/sh

target="../ubx_rdk/bin/rfs_sim.ubifs"

echo -e "\n---------- start ---"

./mkfs.ubifs -r rfs/ -F -o ubifs.img -m 2KiB -e 124KiB -c 892
./ubinize -o $target -m 2KiB -p 128KiB -s 512 -O 2048 ./ubinize.cfg
rm ubifs.img

echo -e "$target done! ---\n"


# SP 128KiB
# SL 124KiB
# P  113MiB/128KiB = 904
# B  9
# O  4KiB
# UBI overhead = (9+4) * 128 KiB + 4 KiB * (904-9-4) = 1664 KiB + 3564 KiB = 5228 KiB -> 40 PEBs
# 864 PEBs (110592 KiB)
# 110592 KiB/ 124 KiB = 892, 108MiB

# 0x1800000
# 0x29e0000
