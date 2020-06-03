SysLink Examples
==========================
Look in the platform folders for SysLink examples. Here is a brief
description of each example. Please note, not all examples are available
for each platform.

Extract the examples in a folder/directory of choice and update the paths the
the nessessary components in the products.mak for each of the examples.

SysLink must be configured and built for the device/platform being used. Run
"make syslink" from the top-level SysLink installation before buiding the
examples.

The "make install" target in each of the examples requires the slaveloader
application to be pre-built.  Run "make samples-hlos" from the top-level
SysLink installation before executing "make install" in each of the examples.

==========================
ex01_helloworld         Simple one-shot notify event from slave to host.
ex02_messageq           Use MessageQ module between host and slave.
