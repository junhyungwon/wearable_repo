#   
#   @file   Makefile
#
#   @brief  This uses Kbuild i.e. kernel side build system for the sample
#           application
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

# If KERNELRELEASE is undefined, then this makefile has been invoked
# directly from the command line. Invoke the kernel build system.
ifeq ($(KERNELRELEASE),)


# Update these macros to reflect you environment.
#
# KERNEL = the Linux kernel source directory
# CROSS_COMPILE = path and compiler prefix
#
KERNEL =
CROSS_COMPILE = 


MAKE_OPTS = ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)

# Invoke the kernel build system
.PHONY: driver clean

driver:
	$(MAKE) -C $(KERNEL) M=$(CURDIR) $(MAKE_OPTS) modules

clean:
	$(MAKE) -C $(KERNEL) M=$(CURDIR) $(MAKE_OPTS) clean
	rm -f modules.order

# If KERNELRELEASE is defined, then this makefile has been invoked
# from the kernel build system. Use native build language.
else 

#MODULE := notifyapp

#$(MODULE)-objs := NotifyAppDrv.o
#$(MODULE)-objs += NotifyApp.o

obj-m = notifyapp.o
notifyapp-objs = NotifyApp.o NotifyAppDrv.o  SysLinkSamples.o

endif
