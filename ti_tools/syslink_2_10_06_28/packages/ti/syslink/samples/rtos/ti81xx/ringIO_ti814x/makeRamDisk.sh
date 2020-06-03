#!/bin/bash
#   
#   @file   makeRamDisk.sh
#
#   @brief  Script to create a ramdisk
#
#
#   ============================================================================
#
#   Copyright (c) 2008-2012, Texas Instruments Incorporated
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#   
#   *  Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   
#   *  Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   
#   *  Neither the name of Texas Instruments Incorporated nor the names of
#      its contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#   
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#   Contact information for paper mail:
#   Texas Instruments
#   Post Office Box 655303
#   Dallas, Texas 75265
#   Contact information: 
#   http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
#   DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
#   ============================================================================
#   
# Size of RAM disk (needs to be larger than total slave executables)
size=20971520

ramfilename='RingIORamDisk'
sourcefilename='RingIORamDisk.c'
arraysymbolname='RamDiskArray'

#dspfilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_dsp/whole_program_debug/ringio_ti814x_dsp.xe674'
#videofilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_videom3/whole_program_debug/ringio_ti814x_videom3.xem3'
#vpssfilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_vpssm3/whole_program_debug/ringio_ti814x_vpssm3.xem3'

dspfilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_dsp/debug/ringio_ti814x_dsp.xe674'
videofilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_videom3/debug/ringio_ti814x_videom3.xem3'
vpssfilename='./slaveBuild/ti_syslink_samples_rtos_platforms_ti814x_vpssm3/debug/ringio_ti814x_vpssm3.xem3'

echo "#"
echo "# Creating RamDisk with name $ramfilename of size $size.."
mkimage.x86U $ramfilename $size
echo "# Image created!"

echo "#"
echo "# Copying slave executables to Ramdisk image :$ramfilename"
cptoimage.x86U $ramfilename $dspfilename dsp.out
cptoimage.x86U $ramfilename $videofilename video.out
cptoimage.x86U $ramfilename $vpssfilename vpss.out
echo "# Executables copied!"

echo "#"
echo "# Converting $ramfilename to  $sourcefilename"
imagetosrc.x86U $ramfilename $sourcefilename $arraysymbolname
echo "# Done!"
