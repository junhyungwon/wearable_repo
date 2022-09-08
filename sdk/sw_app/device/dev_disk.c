/*
 * File : dev_disk.c
 *
 * Copyright (C) 2014 UDWORKs
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/mount.h> //# defines BLKGETSIZE64
#include <ctype.h>		/* for isdigit() */
#include <errno.h>		/* ERANGE */
#include <blkid/blkid.h>
#include <sys/ioctl.h>

#include "dev_disk.h"
#include "dev_common.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//#define DISK_DBG

#ifdef DISK_DBG
#define disk_dbg(fmt, arg...)  	fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, ## arg)
#else
#define disk_dbg(fmt, arg...)  	do { } while (0)
#endif

#define KB				(1024)
#define HDIO_GETGEO 	0x0301

//# About system Ids
#define EMPTY_PARTITION		0
#define EXTENDED_PARTITION	5
#define WIN98_EXTENDED		0x0f
#define DM6_AUX1PARTITION	0x51
#define DM6_AUX3PARTITION	0x53
#define DM6_PARTITION		0x54
#define EZD_PARTITION		0x55
#define LINUX_SWAP			0x82
#define LINUX_NATIVE		0x83
#define LINUX_EXTENDED		0x85
#define BSD_PARTITION		0xa5
#define NETBSD_PARTITION	0xa9

#define CDROM_DEV			"/dev/scd0"
#define _PATH_DEV_BYID		"/dev/disk/by-id"
#define _PATH_DEV_BYPATH	"/dev/disk/by-path"

#define MOUNT_PROC_PATH		"/proc/mounts"
#define PROC_PARTITIONS 	"/proc/partitions"

#define FS_STR_EXT3			"ext3"
#define FS_STR_EXT4			"ext4"

#define MMC_BLK_NAME		"mmcblk"
#define MMC_MB				(1024*1024)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
struct systypes {
	unsigned char type;
	char *name;
};

static struct systypes i386_sys_types[] = {
	{0x00, "Empty"},
	{0x01, "FAT12"},
	{0x02, "XENIX root"},
	{0x03, "XENIX usr"},
	{0x04, "FAT16 <32M"},
	{0x05, "Extended"},		/* DOS 3.3+ extended partition */
	{0x06, "FAT16"},		/* DOS 16-bit >=32M */
	{0x07, "HPFS/NTFS"},	/* OS/2 IFS, eg, HPFS or NTFS or QNX */
	{0x08, "AIX"},		/* AIX boot (AIX -- PS/2 port) or SplitDrive */
	{0x09, "AIX bootable"},	/* AIX data or Coherent */
	{0x0a, "OS/2 Boot Manager"},/* OS/2 Boot Manager */
	{0x0b, "W95 FAT32"},
	{0x0c, "W95 FAT32 (LBA)"},/* LBA really is `Extended Int 13h' */
	{0x0e, "W95 FAT16 (LBA)"},
	{0x0f, "W95 Ext'd (LBA)"},
	{0x10, "OPUS"},
	{0x11, "Hidden FAT12"},
	{0x12, "Compaq diagnostics"},
	{0x14, "Hidden FAT16 <32M"},
	{0x16, "Hidden FAT16"},
	{0x17, "Hidden HPFS/NTFS"},
	{0x18, "AST SmartSleep"},
	{0x1b, "Hidden W95 FAT32"},
	{0x1c, "Hidden W95 FAT32 (LBA)"},
	{0x1e, "Hidden W95 FAT16 (LBA)"},
	{0x24, "NEC DOS"},
	{0x39, "Plan 9"},
	{0x3c, "PartitionMagic recovery"},
	{0x40, "Venix 80286"},
	{0x41, "PPC PReP Boot"},
	{0x42, "SFS"},
	{0x4d, "QNX4.x"},
	{0x4e, "QNX4.x 2nd part"},
	{0x4f, "QNX4.x 3rd part"},
	{0x50, "OnTrack DM"},
	{0x51, "OnTrack DM6 Aux1"},	/* (or Novell) */
	{0x52, "CP/M"},		/* CP/M or Microport SysV/AT */
	{0x53, "OnTrack DM6 Aux3"},
	{0x54, "OnTrackDM6"},
	{0x55, "EZ-Drive"},
	{0x56, "Golden Bow"},
	{0x5c, "Priam Edisk"},
	{0x61, "SpeedStor"},
	{0x63, "GNU HURD or SysV"},	/* GNU HURD or Mach or Sys V/386 (such as ISC UNIX) */
	{0x64, "Novell Netware 286"},
	{0x65, "Novell Netware 386"},
	{0x70, "DiskSecure Multi-Boot"},
	{0x75, "PC/IX"},
	{0x80, "Old Minix"},	/* Minix 1.4a and earlier */
	{0x81, "Minix / old Linux"},/* Minix 1.4b and later */
	{0x82, "Linux swap / Solaris"},
	{0x83, "Linux"},
	{0x84, "OS/2 hidden C: drive"},
	{0x85, "Linux extended"},
	{0x86, "NTFS volume set"},
	{0x87, "NTFS volume set"},
	{0x88, "Linux plaintext"},
	{0x8e, "Linux LVM"},
	{0x93, "Amoeba"},
	{0x94, "Amoeba BBT"},	/* (bad block table) */
	{0x9f, "BSD/OS"},		/* BSDI */
	{0xa0, "IBM Thinkpad hibernation"},
	{0xa5, "FreeBSD"},		/* various BSD flavours */
	{0xa6, "OpenBSD"},
	{0xa7, "NeXTSTEP"},
	{0xa8, "Darwin UFS"},
	{0xa9, "NetBSD"},
	{0xab, "Darwin boot"},
	{0xaf, "HFS / HFS+"},
	{0xb7, "BSDI fs"},
	{0xb8, "BSDI swap"},
	{0xbb, "Boot Wizard hidden"},
	{0xbe, "Solaris boot"},
	{0xbf, "Solaris"},
	{0xc1, "DRDOS/sec (FAT-12)"},
	{0xc4, "DRDOS/sec (FAT-16 < 32M)"},
	{0xc6, "DRDOS/sec (FAT-16)"},
	{0xc7, "Syrinx"},
	{0xda, "Non-FS data"},
	{0xdb, "CP/M / CTOS / ..."},/* CP/M or Concurrent CP/M or
					   Concurrent DOS or CTOS */
	{0xde, "Dell Utility"},	/* Dell PowerEdge Server utilities */
	{0xdf, "BootIt"},		/* BootIt EMBRM */
	{0xe1, "DOS access"},	/* DOS access or SpeedStor 12-bit FAT
					   extended partition */
	{0xe3, "DOS R/O"},		/* DOS R/O or SpeedStor */
	{0xe4, "SpeedStor"},	/* SpeedStor 16-bit FAT extended
					   partition < 1024 cyl. */
	{0xeb, "BeOS fs"},
	{0xee, "GPT"},		/* Intel EFI GUID Partition Table */
	{0xef, "EFI (FAT-12/16/32)"},/* Intel EFI System Partition */
	{0xf0, "Linux/PA-RISC boot"},/* Linux/PA-RISC boot loader */
	{0xf1, "SpeedStor"},
	{0xf4, "SpeedStor"},	/* SpeedStor large partition */
	{0xf2, "DOS secondary"},	/* DOS 3.3+ secondary */
	{0xfb, "VMware VMFS"},
	{0xfc, "VMware VMKCORE"},	/* VMware kernel dump partition */
	{0xfd, "Linux raid autodetect"},/* New (2.2.x) raid partition with
					       autodetect using persistent
					       superblock */
	{0xfe, "LANstep"},		/* SpeedStor >1024 cyl. or LANstep */
	{0xff, "BBT"},		/* Xenix Bad Block Table */
	{ 0, 0 }
};

typedef struct {
	unsigned char h;
	unsigned char s;
	unsigned char c;
} __attribute__((packed)) chs; /* has some c bits in s */

struct partition {
    unsigned char bootable;		/* 0 or 0x80 */
    chs begin_chs;
    unsigned char sys_type;
    chs end_chs;
    unsigned int start_sect;	/* starting sector counting from 0 */
    unsigned int nr_sects;	/* nr of sectors in partition */
} __attribute__((packed));

struct part_desc {
    unsigned long start;
    unsigned long size;
    unsigned long sector, offset; /* disk location of this info */

    struct partition p;
    struct part_desc *ep;  /* extended partition containing this one */
    int ptype;

#define DOS_TYPE	0
#define BSD_TYPE	1
};

struct disk_desc {
    struct part_desc partitions[512];
} oldp;

struct sector {
    struct sector *next;
    unsigned long sectornumber;
    int to_be_written;
    char data[512];

} *sectorhead;

struct geometry {
	unsigned long long total_size;		/* in sectors */
	unsigned long cylindersize;		/* in sectors */
	unsigned long heads, sectors, cylinders;
	unsigned long start;
} B, F, U;

struct hd_geometry {
	unsigned char heads;
	unsigned char sectors;
	unsigned short cylinders;	/* truncated */
	unsigned long start;
};

typedef struct {
	char dev[SIZE_DEV_NAME];	//# device name
	char dir[256];				//# mounted directory
	char type[16];				//# filesystem type
	long sz_total;
	long sz_avail;
	long sz_used;
} mdev_info_t;

typedef struct {
	int cnt;
	mdev_info_t disk[MAX_DISK_NUM];
} mount_info_t;

typedef struct {
	char 			dev[SIZE_DEV_NAME];	//# partition device name
	unsigned char	stype;				//# partition sys_type
	unsigned long	size;				//# partition size(MB)
} part_info_t;

typedef struct {
	char 			dev[SIZE_DEV_NAME];	//# disk device name
	unsigned long	size;				//# disk size(MB)
	int				pno;				//# number of partition
	part_info_t		part[MAX_PARTITION];//# info of partition
} dev_info_t;

typedef struct {
	int dno;	//# number of disk
	dev_info_t	disk[MAX_DISK_NUM];

} disk_info_t;

struct mmc_part_info {
	int part_no;
	int part_type;
	unsigned long part_size; //# MB unit.
};

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
#ifdef DISK_DBG
//# for print disk size - round down
static char str_size[16];
static char *cnv_str(unsigned long megabyte)
{
	if (megabyte < 1000) {
		sprintf(str_size, "%ldMB", megabyte);
	} else {
		sprintf(str_size, "%ld.%ldGB", ((megabyte/100)/10), ((megabyte/100)%10));
	}

	return (char *)&str_size;
}
#endif

static int mmc_is_inserted(void)
{
	FILE *procfs;

	char buf[256] = {0,};
	char dev[128] = {0,};

	procfs = fopen(PROC_PARTITIONS, "r");
	if (procfs == NULL) {
		dev_err("%s open failed\n", PROC_PARTITIONS);
		return 0;
	}

	while (fgets(buf, 255, procfs)) {
		/* %*s->discard input */
		sscanf(buf, "%*s%*s%*s%s", dev);
		/* dev = mmcblk0 -> -1*/
		if (strncmp(dev, MMC_BLK_NAME, strlen(dev)-1) == 0) {
			fclose(procfs);
			return 1;
		}
		dev_dbg("dev %s(=%s)\n", dev, MMC_BLK_NAME);
	}
	fclose(procfs);

	return 0;
}

static char *partname(char *dev, int pno)
{
	static char bufp[80];
	char *p;
	int w, wp;

	w = strlen(dev);
	p = "";

	if (isdigit(dev[w-1]))
		p = "p";

	if (strcmp (dev + w - 4, "disc") == 0) {
		w -= 4;
		p = "part";
	}

	if ((strncmp(dev, _PATH_DEV_BYID, strlen(_PATH_DEV_BYID)) == 0) ||
	     strncmp(dev, _PATH_DEV_BYPATH, strlen(_PATH_DEV_BYPATH)) == 0) {
	       p = "-part";
	}

	wp = strlen(p);
	snprintf(bufp, sizeof(bufp), "%*.*s%s%u", 10/*lth*/-wp-2, w, dev, p, pno);

	return bufp;
}

static const char *sysname(unsigned char type)
{
	struct systypes *s;

	for (s = i386_sys_types; s->name; s++) {
		if (s->type == type)
			return s->name;
	}

	return "Unknown";
}

static int copy_to_int(unsigned char *cp)
{
    unsigned int m;

    m = *cp++;
    m += (*cp++ << 8);
    m += (*cp++ << 16);
    m += (*cp++ << 24);

    return m;
}

static void copy_to_part(char *cp, struct partition *p)
{
    p->bootable = *cp++;
    p->begin_chs.h = *cp++;
    p->begin_chs.s = *cp++;
    p->begin_chs.c = *cp++;
    p->sys_type = *cp++;
    p->end_chs.h = *cp++;
    p->end_chs.s = *cp++;
    p->end_chs.c = *cp++;

    p->start_sect = copy_to_int((unsigned char *) cp);
    p->nr_sects = copy_to_int((unsigned char *) cp+4);
}

static int msdos_signature(struct sector *s)
{
    unsigned char *data = (unsigned char *)s->data;

    if (data[510] == 0x55 && data[511] == 0xaa)
	    return 1;

    dev_err("ERROR: sector %lu does not have an msdos signature\n",
    						s->sectornumber);
    return 0;
}

static int sseek(char *dev, unsigned int fd, unsigned long s)
{
    off_t in, out;

    in = ((off_t) s << 9);
    out = 1;

    if ((out = lseek(fd, in, SEEK_SET)) != in) {
		dev_err("seek error on %s - cannot seek to %lu\n", dev, s);
		return 0;
    }

    if (in != out) {
		dev_err("seek error\n");
		return 0;
    }

    return 1;
}

static struct sector *get_sector(char *dev, int fd, unsigned long sno)
{
    struct sector *s;

    for (s = sectorhead; s; s = s->next)
		if (s->sectornumber == sno)
		    return s;

    if (!sseek(dev, fd, sno))
		return 0;

    if (!(s = (struct sector *)malloc(sizeof(struct sector))))
		dev_err("out of memory - giving up\n");

    if (read(fd, s->data, sizeof(s->data)) != sizeof(s->data)) {
		dev_err("read error on %s - cannot read sector %lu\n", dev, sno);
		free(s);
		return 0;
    }

    s->next = sectorhead;
    sectorhead = s;
    s->sectornumber = sno;
    s->to_be_written = 0;

    return s;
}

static void free_sectors(void)
{
    struct sector *s;

    while (sectorhead) {
		s = sectorhead;
		sectorhead = s->next;
		free(s);
    }
}

static int get_partitions(disk_info_t *idisk, char *dev, int num)
{
	struct partition pt;
    struct sector *s;

    struct disk_desc *z = &oldp;
    struct part_desc *partitions = &(z->partitions[0]);

	int fd, i;
    char *cp;
	int pno = 0;

    unsigned long size;

    char devname[SIZE_DEV_NAME];

	//d_printf("get_partitions %s\n", dev);
	fd = open(dev, O_RDONLY);
	if (fd < 0) {
		dev_err("cannot open %s\n", dev);
		return -1;
	}

	free_sectors();
	if (!(s = get_sector(dev, fd, 0)))
		goto exit;

    if (!msdos_signature(s))
		goto exit;

    cp = s->data + 0x1be;

    for (pno = 0; pno < MAX_PARTITION; pno++, cp += sizeof(struct partition)) {
    	partitions[pno].sector = 0;
		partitions[pno].offset = cp - s->data;

    	copy_to_part(cp, &pt);

    	if(pt.nr_sects == 0)
    		break;

		partitions[pno].start = 0 + pt.start_sect;
		partitions[pno].size = pt.nr_sects;
		partitions[pno].ep = 0;
		partitions[pno].p = pt;
    }

    for (i = 0; i < pno; i++) {
    	sprintf(devname, "%s", partname(dev, i+1));
    	size = (partitions[i].size)/2;	//# KB
    	//d_printf("  > %s\t", devname, cnv_str(size));
		//d_printf("\t0x%02X(%s)\n", partitions[i].p.sys_type, sysname(partitions[i].p.sys_type));

    	memcpy(idisk->disk[num].part[i].dev, devname, sizeof(devname));

    	idisk->disk[num].part[i].stype = partitions[i].p.sys_type;
		idisk->disk[num].part[i].size = size;
    }

    idisk->disk[num].pno = pno;

exit:
    close(fd);

	return 0;
}

/* get 512-byte sector count */
static int blkdev_get_size(const char *name,
					unsigned long long *size)
{
	int fd = -1, ret = 0;

	unsigned long long bytes;

	fd = open(name, O_RDONLY);
	if (fd < 0)
		return -1;

	ret = ioctl(fd, BLKGETSIZE64, &bytes);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	//*sectors = (bytes >> 9);
	*size = bytes/1000;	//# return kbyte

	return 0;
}

static int is_whole_disk(const char *name)
{
	struct hd_geometry geometry;

	int fd = -1;
	int ret = 0;

	fd = open(name, O_RDONLY);
	if (fd != -1) {
		ret = ioctl(fd, HDIO_GETGEO, &geometry); //# get device geometry.
		close(fd);
	}

	if (ret == 0)
		return geometry.start == 0;

	while (*name)
		name++;

	return !isdigit(name[-1]);
}

static int get_disk_info(disk_info_t *idisk)
{
	FILE *proc_f = NULL;

	char buf[256 + 1] = {};
	char ptname[128] = {};

	char devname[SIZE_DEV_NAME] = {};

	int ma, mi;
	int cnt = 0;

	unsigned long long sz;

	proc_f = fopen(PROC_PARTITIONS, "r");
	if (proc_f == NULL) {
		dev_err("cannot open %s\n", PROC_PARTITIONS);
		return -1;
	}

	/* major minor  #blocks  name
  	 * 	31     0     128 	 mtdblock0
 	 *  179    0   30915584  mmcblk0
 	 *  179    1   30915552  mmcblk0p1
     */
	while (fgets(buf, 256, proc_f)) {
		if (sscanf (buf, " %d %d %llu %128[^\n ]", &ma, &mi, &sz, ptname) != 4)
			continue;

		if (!strncmp(ptname, "mtd", 3))		//# skip mtdblockx partitions
			continue;

		snprintf(devname, sizeof(devname), "/dev/%s", ptname);
		if (!is_whole_disk(devname))
			continue;

		blkdev_get_size(devname, &sz);
		//d_printf("* %s (%s)\n", devname, cnv_str(sz));

		if (get_partitions(idisk, devname, cnt))
			continue;

		memcpy(idisk->disk[cnt].dev, devname, sizeof(devname));
		idisk->disk[cnt].size = sz;
		cnt++;
	}
	fclose(proc_f);

#if 0
	if(get_cdrom_info(devname)) {
		memcpy(idisk->disk[cnt].dev, devname, sizeof(devname));
		cnt++;
	}
#endif
	idisk->dno = cnt;

	return 0;
}

static int get_df_info(FILE *fp, mdev_info_t *mp)
{
	struct statfs lstatfs;
	struct stat lstat;

	char buf[256] = {};
	int is_dev = 0;

	while(fgets(buf, 255, fp)) {
		is_dev = 0;

		sscanf(buf, "%s%s%s", mp->dev, mp->dir, mp->type);
		if (strncmp(mp->dev,"/dev/", 5) == 0)
			is_dev = 1;

		if (strcmp(mp->type, "nfs") == 0) {
			if (strcmp(mp->dir, "/") == 0)
				return 2;
		}

		if (stat(mp->dev, &lstat) == 0 && is_dev)
		{
			if ((strstr(buf, mp->dir) && S_ISBLK(lstat.st_mode)) || is_dev) {
				statfs(mp->dir, &lstatfs);
				mp->sz_total = lstatfs.f_blocks * (lstatfs.f_bsize/KB);
				mp->sz_avail = lstatfs.f_bavail * (lstatfs.f_bsize/KB);
				mp->sz_used = mp->sz_total - (lstatfs.f_bfree * (lstatfs.f_bsize/KB));
				return 1;
			}
		}
	}

	return 0;
}

static int get_mount_info(mount_info_t *mp, int *rfs_type)
{
	FILE *fp;

	int ret = 0;
	int is_nfs = 0;

	fp = fopen(MOUNT_PROC_PATH, "r");
	if (fp == NULL) {
		return -1;
	}

	mp->cnt = 0;
	while (1) {
		ret = get_df_info(fp, &mp->disk[mp->cnt]);
		if (ret == 1)
			mp->cnt++;
		else if (ret == 2)
			is_nfs = 1;
		else
			break;
	}
	fclose(fp);

	*rfs_type = is_nfs;

	return 0;
}

/*****************************************************************************
* @brief	get_dvr_disk_info function
* @section	DESC Description
*	- desc
*****************************************************************************/
static int search_mount_info(device_info_t *idev, char *devname, unsigned char stype,
						mount_info_t *imount, int state)
{
	int i, fmount;
	mdev_info_t *minfo;

	fmount = 0;

	for(i = 0; i < imount->cnt; i++) {
		minfo = &imount->disk[i];

		if (!strcmp(devname, minfo->dev)) {
			memcpy(idev->dev, minfo->dev, SIZE_DEV_NAME);
			memcpy(idev->dir, minfo->dir, SIZE_DIR_NAME);
			memcpy(idev->type, minfo->type, SIZE_TYP_NAME);

			idev->sz_total = minfo->sz_total;
			idev->sz_used = minfo->sz_used;
			idev->sz_avail = minfo->sz_avail;

			if (stype == LINUX_NATIVE)
			{
				if (strcmp(idev->type, FS_STR_EXT3) == 0)
					idev->state = DONE_EXT3;
				else if (strcmp(idev->type, FS_STR_EXT4) == 0)
					idev->state = DONE_EXT4;
			} else {
				idev->state = state;
			}
			fmount = 1;
			break;
		}
	}

	return fmount;
}

/****************************************************
 * NAME : int dev_disk_get_info(dev_disk_info_t *ddi)
 *
 * Desc : Get disk information (partition.)
 *
 ****************************************************/
int dev_disk_get_info(dev_disk_info_t *ddi)
{
	int ret = 0;

	disk_info_t idisk;
	dev_info_t *dinfo;
	part_info_t *pinfo;
	mount_info_t imount;

#if 0
	mdev_info_t *minfo;
#endif

	int rfs_type = 0;
	int i, j, cnt = 0;
	int fmount;

	memset((void *)&idisk, 0, sizeof(disk_info_t));
	memset((void *)&imount, 0, sizeof(mount_info_t));

	ret = get_disk_info(&idisk);
	if (ret < 0)
		return -1;

	ret = get_mount_info(&imount, &rfs_type);
	if (ret < 0)
		return -1;

	for (i = 0; i < idisk.dno; i++) {
		fmount = 0;
		dinfo = &idisk.disk[i];

		disk_dbg("%-16s %+8s, partition %d\n", dinfo->dev, cnv_str(dinfo->size), dinfo->pno);

		if (!dinfo->pno) {
			fmount = search_mount_info(&ddi->dev[cnt], dinfo->dev, 0, &imount, DISK_CDROM);

			if (!fmount) {
				memcpy(ddi->dev[cnt].dev, dinfo->dev, SIZE_DEV_NAME);
				ddi->dev[cnt].sz_total = dinfo->size;

				if (!strcmp(dinfo->dev, CDROM_DEV))
					ddi->dev[cnt].state = DISK_CDROM;
				else
					ddi->dev[cnt].state = DISK_IDLE;
			}
			cnt++;
		}

		for (j = 0; j < dinfo->pno; j++) {
			pinfo = &dinfo->part[j];

			disk_dbg(" %-15s %+8s \t0x%02X(%s)\n",
				pinfo->dev, cnv_str(pinfo->size), pinfo->stype, sysname(pinfo->stype));

			fmount = search_mount_info(&ddi->dev[cnt], pinfo->dev, pinfo->stype, &imount, DONE_FORMAT);
			if (!fmount) {
				memcpy(ddi->dev[cnt].dev, pinfo->dev, SIZE_DEV_NAME);
				memcpy(ddi->dev[cnt].type, sysname(pinfo->stype), SIZE_TYP_NAME);
				ddi->dev[cnt].sz_total = pinfo->size;
				ddi->dev[cnt].state = DONE_FDISK;
			}
			cnt++;
		}
	}

	#if 0
	disk_dbg("----------------------------\n")
	disk_dbg("root file system %s, disk found %d\n", rfs_type?"nfs":"other", imount.cnt);
	for (i = 0; i < imount.cnt; i++) {
		minfo = &imount.disk[i];
		disk_dbg("%-20s %-20s %-10s %12lu %12lu %12lu\n", minfo->dev, minfo->dir, minfo->type,
				minfo->sz_total/KB, minfo->sz_used/KB, minfo->sz_avail/KB);
	}
	#endif

	ddi->cnt = cnt;
	ddi->rfs_type = rfs_type;

	return 0;
}

/****************************************************
 * NAME : int dev_disk_get_size(char *mount_name, unsigned long *total,
					unsigned long *used)
 ****************************************************/
int dev_disk_get_size(char *mount_name, unsigned long *total,
					unsigned long *used)
{
	struct statfs lstatfs;

	unsigned long sz_total = 0;
	unsigned long sz_avail = 0;
	unsigned long sz_used = 0;

	if (statfs(mount_name, &lstatfs) < 0)
		return -1;

	sz_total = lstatfs.f_blocks * (lstatfs.f_bsize/KB);
	sz_avail = lstatfs.f_bavail * (lstatfs.f_bsize/KB);
	sz_used = sz_total - (lstatfs.f_bavail * (lstatfs.f_bsize/KB));

	*total = sz_total;
	*used = sz_used;

	return 0;
}



/*****************************************************************************
 * @brief    mmc partition check function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int dev_disk_mmc_check(void)
{
	blkid_probe pr;
	blkid_partlist ls;
	blkid_partition par;

	struct mmc_part_info mmc_part = {
		.part_no = 0,
		.part_type = 0,
		.part_size = 0
	};
	int nparts, i;
	
	if (mmc_is_inserted() == 0) {
		dev_err("sd card is not inserted\n");
		return -1;
	}

	pr = blkid_new_probe_from_filename("/dev/mmcblk0");
	if (!pr) {
		dev_err("Faild to create a new libblkid probe on /dev/mmcblk0");
		return -1;
	}

	/* Binary interface */
	ls = blkid_probe_get_partitions(pr);
	/*
	 * List partitions
	 */
	nparts = blkid_partlist_numof_partitions(ls);
	/* not supported multi partitions */
	if (!nparts) {
		blkid_free_probe(pr);
		return -1;
	}

#if 0
	/* %j -> C99 intmax_t */
	dev_dbg("size: %jd\n", blkid_probe_get_size(pr));
#endif

	par = blkid_partlist_get_partition(ls, 0); //# fixed 0
	blkid_free_probe(pr);
	
	mmc_part.part_no = nparts;
	mmc_part.part_type = blkid_partition_get_type(par);
	mmc_part.part_size = (unsigned long)(blkid_probe_get_size(pr) / MMC_MB);
	
	 /* 128GB or multi partition */
	if ((mmc_part.part_size > 131072) || (mmc_part.part_no != 1)) {
		dev_err("Not suppoted MMC card (%lu MB, %d)!!\n", mmc_part.part_size, mmc_part.part_no);
		return -1;
	}
	
	 /* format from exFAT to VFAT */
	if (mmc_part.part_type == 0x7) 
	{
		DIR *mount_dir = NULL;
		/* A system reset is required after partition delete. */
		/* To avoid reset, repeat partition delete. */
		/* fdisk delete */
		system("/sbin/fdisk /dev/mmcblk0 <<'EOF'\nd\nw\nEOF");
		usleep(500000); //# wait for done!
		/* create partition delete exFAT->Y , change->t, FAT32 LBA->c */
		system("/sbin/fdisk /dev/mmcblk0 <<'EOF'\nn\np\n\n\n\nY\nt\nc\nw\nEOF");
		usleep(500000); //# wait for done!
		system("/sbin/mkfs.fat -s 64 -F 32 /dev/mmcblk0p1");
		//# check /mmc directory.
		mount_dir = opendir("/mmc");
		if (mount_dir == NULL) 
			mkdir("/mmc", 0775);
		else
			closedir(mount_dir);
		sync();
	}

	return 0;
}

/*****************************************************************************
 * @brief    int dev_disk_mmc_check_writable(void)
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int dev_disk_mmc_check_writable(void)
{
	FILE *f=NULL;
	char buf[256 + 1]={0,};
	char mntd[64]={0,};
	
	/* SD 카드 속성확인 (read only file system) */
	f = fopen("/proc/mounts", "r");
	if (f == NULL) {
		/* assume readonly fs */
		return 0;
	}
	
	/* 
	 * normal fs       -> /dev/mmcblk0p1 /mmc vfat rw,noatime,nodiratime,fmask=0000,
	 * if read-only fs -> /dev/mmcblk0p1 /mmc vfat ro,noatime,......
	 */
	while (fgets(buf, 256, f) != NULL) 
	{
		char *tmp, *tmp2;
		/* %*s->discard input */
		sscanf(buf, "%*s%s", mntd);
		if (strcmp(mntd, "/mmc") == 0) {
			tmp = strtok(buf, ",");
			if (tmp != NULL) {
				tmp2 = strstr(tmp, "rw");
				if (tmp2 != NULL) {
					fclose(f);
					return 1;
				}
			}
		}
	}
	
	dev_dbg("sd card read-only filesystem!!\n");
	fclose(f);
	return 0;
}

/****************************************************
 * NAME : int dev_disk_check_mount(const char *mount_point)
 ****************************************************/
int dev_disk_check_mount(const char *mount_point)
{
	FILE *procfs = NULL;

	char line[256 + 1] = {0,};
	char proc_name[256 + 1] = {0,};

	int found = 0, length;

	procfs = fopen(MOUNT_PROC_PATH, "r");
	if (procfs == NULL) {
		dev_err("couldn't open %s\n", MOUNT_PROC_PATH);
		return 0; //# assume device is not mount.
	}

	length = strlen(mount_point);
	while (fgets (line, 256, procfs)) {
		sscanf(line, "%*s%s", proc_name);
		if (strncmp(proc_name, mount_point, length) == 0) {
		    found = 1;
		    break; //# founded
		}
	}
    fclose(procfs);

	return (found ? 1: 0);
}
