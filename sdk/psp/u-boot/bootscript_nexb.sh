#
# boot script for fitt
#
echo "update NAND"

#nand scrub
#--- u-boot --------------------------------------
mw.b 0x81000000 0xFF 0x20000
if mmc rescan;fatload mmc 0 0x81000000 u-boot_fit.min.nand; then
	nandecc hw 2;nand erase 0x0 0x20000;nand write.i 0x81000000 0x0 0x20000
fi

mw.b 0x81000000 0xFF 0x30000
if mmc rescan;fatload mmc 0 0x81000000 u-boot_fit.bin; then
	nand erase 0x020000 0xA0000;nand write.i 0x81000000 0x20000 0x30000
fi

#init env
nand erase 0x0C0000 0x20000

#--- kernel --------------------------------------
mw.b 0x81000000 0xFF 0x300000
if mmc rescan;fatload mmc 0 0x81000000 uImage_fit; then
	nand erase 0x00220000 0x300000;nand write 0x81000000 0x00220000 ${filesize}
fi

#--- ubifs ---------------------------------------
#nand scrub 0x00520000 0x46E0000
#nand scrub 0x00520000 0x46E0000
if mmc rescan;fatload mmc 0 0x81000000 rfs_fit.ubifs; then
	nand erase 0x00520000 0x46E0000;nand write 0x81000000 0x00520000 ${filesize}
fi

#--- Data Area -----------------------------------
nand scrub 0x04C00000 0x3000000

setenv bootcmd 'run update;date valid;nboot.e 0x81000000 0 0x220000;bootm 0x81000000;'
setenv bootargs 'mem=180M console=ttyO0,115200n8 noinitrd rw ubi.mtd=5,2048 rootfstype=ubifs root=ubi0:rootfs ip=off vram=6M notifyk.vpssm3_sva=0xBFD00000 quiet' eth=${ethaddr}

#--- save env ---
setenv nand_update 2
saveenv

#--- mcu -----------------------------------------
if mmc rescan;fatload mmc 0 0x81000000 mcu_nexb.txt; then
	mcu update
fi

#--- reboot ---
re
