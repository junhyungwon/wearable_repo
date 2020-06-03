ECHO off
rem 
rem @file   makeRamDisk.bat
rem
rem   @brief  Script to create a ramdisk
rem
rem
rem ============================================================================
rem
rem Copyright (c) 2008-2012, Texas Instruments Incorporated
rem
rem Redistribution and use in source and binary forms, with or without
rem modification, are permitted provided that the following conditions
rem are met:
rem 
rem *  Redistributions of source code must retain the above copyright
rem    notice, this list of conditions and the following disclaimer.
rem 
rem *  Redistributions in binary form must reproduce the above copyright
rem    notice, this list of conditions and the following disclaimer in the
rem    documentation and/or other materials provided with the distribution.
rem 
rem *  Neither the name of Texas Instruments Incorporated nor the names of
rem    its contributors may be used to endorse or promote products derived
rem    from this software without specific prior written permission.
rem 
rem THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
rem AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
rem THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
rem PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
rem CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
rem EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
rem PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
rem OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
rem WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
rem OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
rem EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
rem Contact information for paper mail:
rem Texas Instruments
rem Post Office Box 655303
rem Dallas, Texas 75265
rem Contact information: 
rem http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
rem DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
rem ============================================================================
rem 
REM This file creates an A8 RAM disk with slave executables

REM Set path for FATFS executables and current application directory
SET FATFS_BIN_DIR=%1
SET APP_DIR=%2

REM Size of RAM disk (needs to be larger than total slave exectuables)
SET size=25165824

SET ramfilename=%APP_DIR%\FrameQRamDisk
SET sourcefilename=%APP_DIR%\FrameQRamDisk.c
SET arraysymbolname=RamDiskArray

SET dspfilename=%APP_DIR%\slaveBuild\ti_syslink_samples_rtos_platforms_ti814x_dsp\debug\frameq_ti814x_dsp.xe674
SET videofilename=%APP_DIR%\slaveBuild\ti_syslink_samples_rtos_platforms_ti814x_videom3\debug\frameq_ti814x_videom3.xem3
SET vpssfilename=%APP_DIR%\slaveBuild\ti_syslink_samples_rtos_platforms_ti814x_vpssm3\debug\frameq_ti814x_vpssm3.xem3

ECHO on
REM #
REM # Creating RamDisk with name %ramfilename% of size %size%..
%FATFS_BIN_DIR%\mkimage.exe %ramfilename% %size%
REM # Image created!

REM #
REM # Copying slave executables to Ramdisk image: %ramfilename%
%FATFS_BIN_DIR%\cptoimage.exe %ramfilename% %dspfilename% dsp.out
%FATFS_BIN_DIR%\cptoimage.exe %ramfilename% %videofilename% video.out
%FATFS_BIN_DIR%\cptoimage.exe %ramfilename% %vpssfilename% vpss.out
REM # Executables copied!

REM # 
REM # Converting %ramfilename% to %sourcefilename%
%FATFS_BIN_DIR%\imagetosrc.exe %ramfilename% %sourcefilename% %arraysymbolname%
REM # Done!
