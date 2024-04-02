# BusyBox
BusyBox is a software suite that provides a single binary executable file that contains many common Unix utilities. It is designed to be small and efficient, and is often used in embedded systems or other systems with limited resources.

## Download BusyBox

```bash
# clone busybox
git clone https://github.com/mirror/busybox.git
cd busybox/
```

## Compile BusyBox

```bash
# export the compiler on the system
# chose the right compiler that corespond to your board
export CROSS_COMPILE=arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm

# Configure busybox to be static build from menuconfig
make menuconfig

#build the busybox to get the minimal command
make

# generate the rootfs
make install
# this will create folder name _install has all binary
```


## Create Rootfs
Copy all generated files from **_install** after building busybox and its dependent libraries in the toolchain **sysroot** to the staging directory.


```bash
# create staging directory
mkdir rootfs

# copy the content of _install to rootfs
cp -rp ./busybox/_install/* ~/rootfs
# copy the content of toolchain sysroot to rootfs
# copy files using rsynch to overwriting
rsynch -a ~/<path_to_toolchain_sysroot>  ~/rootfs
# create the rest of directories for rootfs
cd ~/rootfs
mkdir -p dev etc home proc sys tmp
```

### Ownership Permissions
Change Ownership Permissions for the statgeing directory **rootfs** to root to restrict the access to these files as those files are sensitive.

```bash
sudo chown -R root:root ~/rootfs
```


## BusyBox init process

The first program runs is the init process `(/sbin/init)`, as the kernel search for a starting point so init process starts and monitors other programs.

the init process starts by reading a configuration file `/etc/inittab`

### inittab Configuration File

```bash
# inittab file 
::sysinit:/etc/init.d/rcS
# Start an "askfirst" shell on the console
::askfirst:-/bin/sh
```
inittab file runs a shell script `rcS`, when init is started. then ask to press enter to activate the console.


### rcS Script

The script called **/etc/init.d/rcS** is the place to put initialization commands that need to
be performed at boot, such as mounting the proc and sysfs filesystems.

```bash 
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sysfs /sys

```

> Make the script executable 

```bash
cd ~/rootfs/etc/init.d/
sudo chmod +x rcS
```
## Mount Rootfs through SD Card

### Mount SD Card

```bash
# mount the sd card to the host file system
# No need to do this command if the sd card already mounted
sudo mount /dev/loop<x>p1 /sdcard/sd
sudo mount /dev/loop<x>p2 /sdcard/sd2

# copy rootfs into the sd card
cd rootfs
cp -r * ~/sdcard/sd2 
```
 ### Set Bootargs in U-boot

```bash
setenv bootargs 'console=ttyAMA0,115200n8 root=/dev/mmcblk0p2 rootfstype=ext4 rw init=/sbin/init'
# console is set depends on the machine
```

## Mount Rootfs through NFS

### Install NFS

```bash
# Install  NFS server
sudo apt install nfs-kernel-server

# make rootfs directory 
# copy the content of the staging directory in it
mkdir /srv/rootfs
sudo cp -r ~/rootfs /srv/rootfs

# Add exported directory to `/etc/exports` file, with target ip
/srv/rootfs 192.168.1.95(rw,no_root_squash,no_subtree_check)

# Reload the file to apply the new configuration
sudo exportfs -r
```
### Set Bootargs in U-Boot

```bash
setenv bootargs console=ttyAMA0 root=/dev/nfs ip=192.168.1.95:::::eth0 nfsroot=192.168.1.92:/srv/rootfs,nfsvers=3,tcp rw init=/sbin/init
```


# Creating initramfs

`initramfs` is an early filesystem  that is loaded into
RAM by the bootloader before the real root file system becomes available during the linux kernel intialization process.


## Intializing initramfs
An initial RAM filesystem, or initramfs, is a compressed cpio archive. cpio is an old Unix archive format, similar to TAR and ZIP but easier to decode and so requiring less code in the kernel. You need to configure your kernel with CONFIG_BLK_DEV_INITRD to support initramfs.

Make sure do not includes kernel modules in the initramfs as it will take much space.


```bash
cd ~/rootfs
find . | cpio -H newc -ov --owner root:root > ../initramfs.cpio
cd ..
gzip initramfs.cpio
mkimage -A arm -O linux -T ramdisk -d initramfs.cpio.gz uRamdisk
```
:exclamation: Note that we run cpio with the option: **--owner root:root** this is a quick fix for the file
ownership problem mentioned earlier.

## Booting Qenu with intiramfs

QEMU has the option called -initrd to load initramfs into memory

```bash
qemu-system-arm -m 256M -nographic -M vexpress-a9 -kernel sdcard/sd/zImage -append "console=ttyAMA0 rdinit=/bin/sh" -dtb vexpress-v2p-ca9.dtb -initrd initramfs.cpio.gz
```