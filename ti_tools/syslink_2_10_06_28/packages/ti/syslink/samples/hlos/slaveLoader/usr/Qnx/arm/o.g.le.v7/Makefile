#   
#   @file   Makefile
#
#   @brief  QNX common makefile
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
SYSLINK_PROFILE = DEBUG
include ../../common.mk

$(PROJECT_ROOT)/arm/o.g.le.v7/slaveloader_g: $(PROJECT_ROOT)/../../../common/usr/Qnx/arm/a.g.le.v7/libsyslink_common_g.a
$(PROJECT_ROOT)/arm/o.g.le.v7/slaveloader_g: $(SYSLINK_ROOT)/ti/syslink/lib/qnx/libsyslink_g.a

# link with the static version of libsyslink, dynamic for others
LIBPREF_syslink_g = -Bstatic
LIBPOST_syslink_g = -Bdynamic

EXTRA_LIBVPATH += \
        $(PROJECT_ROOT)/../../../common/usr/Qnx/arm/a.g.le.v7 \
        $(SYSLINK_ROOT)/ti/syslink/lib/qnx

LIBS += syslink_common_g
LIBS += syslink_g

POST_BUILD = \
        $(CP_HOST) \
        $(PROJECT_ROOT)/arm/o.g.le.v7/slaveloader_g \
        $(SYSLINK_ROOT)/ti/syslink/bin/qnx/samples/slaveloader_g
