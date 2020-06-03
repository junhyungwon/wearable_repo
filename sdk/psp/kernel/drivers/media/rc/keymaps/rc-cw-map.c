/*
 * Corea Works remote controller keytable
 *
 * Copyright (C) 2014 UDWORKS Inc.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table cw[] = {
	/* Key codes for the KW remote */
	{ 0x10AF00, KEY_RECORD },
	{ 0x10AF68, KEY_MENU },      		//# MENU KEY
	{ 0x10AF80, KEY_PLAYPAUSE }, 		//# toggle (play-pause)
	{ 0x10AF08, KEY_UP },        		//# UP KEY
	{ 0x10AFB0, KEY_UWB },       		//# USB KEY
	{ 0x10AF88, KEY_LEFT },      		//# LEFT KEY
	{ 0x10AFD0, KEY_STOP },      		//# OK/STOP KEY
	{ 0x10AFC8, KEY_RIGHT },     		//# RIGHT KEY
	{ 0x10AF1E, KEY_F1 },        		//# F1 KEY
	{ 0x10AF28, KEY_DOWN },      		//# DOWN KEY
	/* 10 */
	{ 0x10AF1F, KEY_F2 },         		//# F2 KEY
	{ 0x10AFE8, KEY_1 },          		//# 1 KEY
	{ 0x10AFA0, KEY_2 },                //# 2 KEY
	{ 0x10AF48, KEY_SWITCHVIDEOMODE },  //# multi-window
	{ 0x10AF60, KEY_3 },                //# 3 KEY
	{ 0x10AF98, KEY_4 },   				//# 4 KEY
	{ 0x10AFA2, KEY_F3 },   		    //# OSD
};

static struct rc_map_list cw_map = {
	.map = {
		.scan    = cw,
		.size    = ARRAY_SIZE(cw),
		.rc_type = RC_TYPE_NEC,
		.name    = RC_MAP_CW,
	}
};

static int __init init_rc_map_cw(void)
{
	return rc_map_register(&cw_map);
}

static void __exit exit_rc_map_cw(void)
{
	rc_map_unregister(&cw_map);
}

module_init(init_rc_map_cw)
module_exit(exit_rc_map_cw)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UDWORKS");
