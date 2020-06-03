/*
 * dev_disk.h
 *
 * Copyright (C) 2014 UDWORKs.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#ifndef _DEV_DISK_H_
#define _DEV_DISK_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// DVR HDD INFO
#define MAX_DISK_NUM			7 //# sata 4 + sd 1 + usb 2
#define MAX_PARTITION			4
#define MAX_DEVLIST				(MAX_DISK_NUM * MAX_PARTITION)

#define SIZE_DEV_NAME			64
#define SIZE_DIR_NAME			256
#define SIZE_TYP_NAME			128

#define MMC_SDHC_HIGH_SIZE		32768  //# 32GB
#define MMC_SIZE_64GB			65536  //#
#define MMC_SIZE_128GB			131072 //#

#define MMC_MOUNT_POINT			"/mmc"
#define MMC_BLK_DEV_NAME		"/dev/mmcblk0"
#define MMC_PART_NAME			"/dev/mmcblk0p1"

#define MMC_PART_TYPE_EXFAT		0x7
#define MMC_PART_TYPE_FAT32		0xc

typedef struct {
	char dev[SIZE_DEV_NAME];
	char dir[SIZE_DIR_NAME];
	char type[SIZE_TYP_NAME];

	long sz_total;
	long sz_avail;
	long sz_used;

	int  state;
} __attribute__((packed))device_info_t;

typedef struct {
	int cnt;
	int rfs_type;
	int rec_used[MAX_DEVLIST];

	device_info_t dev[MAX_DEVLIST];

} __attribute__((packed))dev_disk_info_t;

typedef enum {
	DISK_IDLE = 0,
	DISK_CDROM,
	DONE_FDISK,
	DONE_FORMAT,
	DONE_EXT3,
	DONE_EXT4,
	MAX_STATE

} DEV_DISK_STATE;

struct mmc_part_info {
	int part_no;
	int part_type;
	unsigned long part_size; //# MB unit.
};

/*
 * Declare Function Prototype.
 */
int dev_disk_get_info(dev_disk_info_t *ddi);
int dev_disk_run_fdisk(const char *dev);
int dev_disk_get_size(char *mount_name, unsigned long *total,
					unsigned long *used);
int dev_disk_mmc_part_check_info(const char *blk_path,
					struct mmc_part_info *part_info);
int dev_disk_mmc_part_unmount(const char *target_path);
int dev_disk_mmc_part_delete(int part_no);
int dev_disk_mmc_part_create(void);
int dev_disk_mmc_part_format(unsigned long size);
int dev_disk_mmc_part_set_bootsector(int part_no, int on);
int dev_disk_mmc_part_set_lba_mode(int part_no, int on);
int dev_disk_check_mount(const char *mount_point);
int dev_disk_find_hdd_path(char *node);
int dev_disk_check_usb_disk_path(char *node);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DEV_DISK_H_ */
