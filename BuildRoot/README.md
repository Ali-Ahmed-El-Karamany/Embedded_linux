# BuildRoot
Buildroot is a buildsystem capable of building the elements of embedded linux: `toolchain`, `Bootloader`, `Kernel`, `root files system`

## Download BuildRoot
### Required Libraries

```bash
sudo apt install sed make binutils gcc g++ bash patch \
gzip bzip2 perl tar cpio python unzip rsync wget libncurses-dev
```

### BuildRoot Source

```bash
#make directory build root 
mkdir buildroot | cd buildroot
# download comperessed buildroot source code 
wget https://buildroot.org/downloads/buildroot-2024.02.tar.gz
# un compress the source code
tar -xvzf buildroot-2024.02.tar.gz
```

## Configure BuildRoot

List all board configurations supported by buildroot 

```bash
ls configs
```


### Configure Qemu

```bash
# list the defconfigs for qemu
ls configs | grep qemu
# configure qemu vexpress
make qemu_arm_vexpress_defconfig
```

### Configure RaspberryPi

```bash
# listing the defconfigs for rasberryPi
ls configs | grep raspberrypi
# configure for raspberryPi 4 64
make raspberrypi4_64_defconfig
```
### Configuree BuildRoot

```bash
# open configuration menu to configure buildroot
make menuconfig
```

In the configuration, configure the following **requirement**:

- [ ]  Init procecss to **System V** or **System D** or **Busybox init**

`In case of using systemD increase the root file system size from build root configuartion`

- [ ]  Change **system host**
- [ ] Change **system banner**
- [ ]  Set **root password**

#### Configure SSH

In order to configure SSH and add the executable in rootfs in **buildroot**

- [ ]  Enable in **Network application** configure **dropbear**

## Build the System

```bash
# It will build the buildroot 
# to proccesses in parallel insert -j4
make -j4

#### WARNING it will take up to 2 hours ####
```

## System Booting for Qemu

```bash
# navigate to to the image directory in buildroot
cd <path-to-buildroot-directory>/output/images 
# run qemu start script to run qemu
./start-qemu.sh
```
### SSH 
To use `ssh` to connect to qemu, you must edit the qemu start script to start a **tap interface** and then set an **IP address** for qemu.

#### Qemu Start Script
```bash
# navigate to the image directory in the buildroot
cd <path-to-buildroot-directory>/output/image
# open qemu start script to edit
sudo vim start-qemu.sh
# edit the qemu-system-arm command
qemu-system-arm -M vexpress-a9 -smp 1 -m 256 -kernel zImage -dtb vexpress-v2p-ca9.dtb -drive file=rootfs.ext2,if=sd,format=raw -append "console=ttyAMA0,115200 rootwait root=/dev/mmcblk0" -net nic,model=lan9118 -net tap,script=<path-to-tap-script>/qemu-ifup
```

#### Start Qemu

```bash
# navigate to to the image directory in buildroot
cd <path-to-buildroot-directory>/output/images 
# run qemu start script to run qemu
./start-qemu.sh
```

#### Set IP Address for qemu
```bash
ip add a 192.168.1.95/24 dev eth0
```

#### Connect with Qemu via SSH from Host Machine
On the host terminal, use SSH to connect to Qemu
```bash
ssh root@192.168.1.95
```

## System Booting for RaspberryPi

```bash
# unmount the sdcard if it is mounted
sudo umount sd<X>
# format the sd card
dd if=/dev/zero of=/dev/sd<X> bs=1M
#### warning it will take some time ####

# copy the Sd image to the SD card
dd if=output/images/sdcard.img of=/dev/sd<x>
```
### SSH


## Custom Package

### Package File Structure
under **package** directory add your custom package directory **helloworld**
```
helloworld
├── Config.in
├── helloworld.mk
└── src
    ├── helloworld.c
    └── Makefile
```
#### Prepare File Structure for the Package

```bash
mkdir package/helloworld
cd package/helloworld

touch Config.in
touch helloworld.mk

mkdir src | cd src  

touch helloworld.c
touch Makefile
```
### Config.in File
`Config.in` used to describe the configuration options for the package

```bash
# open the file to edit
vim Config.in
# insert the configuration into the file
config BR2_PACKAGE_HELLOWORLD
    bool "helloworld"
    help
        A friendly program that prints helloworld.
```
`BR2_PACKAGE_packageName` --> makes the package apears in the configuration option.

`bool "packageNAme"` --> to make the package selectable.

`help` --> it is an option..used to provide help text in configuration option. 

#### :grey_exclamation: To make the package apears in the configuration menu...must add your package Config.in to package/Config.in

```bash
menu "my_package"
    source "package/helloworld/Config.in"
endmenu
```

### helloworld.mk
Provides the data needed by buildroot
```bash
HELLOWORLD_VERSION = 1.0.0
HELLOWORLD_SITE = package/helloworld/src
HELLOWORLD_SITE_METHOD = local

define HELLOWORLD_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define HELLOWORLD_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/helloworld $(TARGET_DIR)/usr/bin/helloworld
endef
```

### helloworld.c
Source code of the package.
```c
#include <stdio.h>

int main() 
{
    printf("Hello, world!\n");
}
```

### Makefile
This used while building. 
```bash
.PHONY: clean
.PHONY: helloworld

helloworld: helloworld.c
	$(CC) -g -Wall $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	-rm helloworld
        
#### NOTE: Makefile requires a tab character for indentation. Spaces will not work. ####
```
