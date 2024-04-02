# U-boot

U-Boot is **the most popular boot loader in** **linux** **based embedded devices**. It is released as open source under the GNU GPLv2 license. It supports a wide range of microprocessors like MIPS, ARM, PPC, Blackfin, AVR32 and x86.

## Setup U-boot

### Download U-Boot

```bash
git clone git@github.com:u-boot/u-boot.git
cd u-boot/
git checkout v2022.07	# Go to a stable commit
```

### Configure U-Boot Machine

In this section we will **configure** u-boot for several Machine

```bash
# to list all the machines supported by u-boot
ls configs
# In order to find the machine supported by U-Boot
ls configs/ | grep [your machine] 
```

### :exclamation: Note: Choose vexpress_ca9x4_defconfig configuration to emulate qemu

### Vexpress Cortex A9 (Qemu)

In `U-boot directory` Assign this value

```bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=<Path To the Compiler>/arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm
# load the default configuration of ARM Vexpress Cortex A9
make vexpress_ca9x4_defconfig
```

### Configure U-Boot 

In this part we need to configure some u-boot configuration for the specific board chosen up.

```bash
make menuconfig
```

**select these custom configurations**:

- [ ] Support **editenv**.
- [ ] Support **bootd**.
- [ ] Store the environment variable inside file call **uboot.env**.
- [ ] Unset support of **Flash**
- [ ] Support **FAT file system**
  - [ ] Configure the FAT interface to **mmc**
  - [ ] Configure the partition where the fat is store to **0:1**
  

### Build U-Boot

After setting up the configurations...it's time to build and make the bootloader to be used!

``` bash
make    # inside the U-Boot Directory!
```

## SD Card
In this section it's required to have SD card with first partition to be FAT as pre configured in `U-boot Menuconfig`.

### :exclamation: ONLY FOR WHO WORK WITH QEMU ->  Must emulate SD card

```bash
# Change directory to where you want to make SD card partition
mkdir ~/path-to-directory/sdcard
# Create an empty file with size 1 GB
dd if=/dev/zero of=sd.img bs=1M count=1024
```
`if` input file. `of` output file `bs` block size. `count` how many times to copy the block size

### Configure the Partition Table for the SD card

```bash
cfdisk sd.img
```

| Partition Size | Partition Format | Bootable  |
| :------------: | :--------------: | :-------: |
|    `200 MB`    |     `FAT 16`     | ***Yes*** |
|    `823 MB`    |     `Linux`      | ***No***  |

#### :exclamation: **do not foget to save** -->  select `write` to apply changes

### Loop Device FOR Virtual usage ONLY

To emulate the sd.img file as sd card we need to attach it to **loop device** to be as a **block storage**

```bash
# attach the sd.img to be treated as block storage
sudo losetup -f --show --partscan sd.img
# Running the upper command will show you
# Which loop device the sd.img is connected  (loop<x>)
```

### Format Partition Table

As pre configured from `cfdisk command` first partition is **FAT**

```bash
# Formating the first partition as FAT
sudo mkfs.vfat -F 16 -n boot loop<x>p1
```
 As pre configured from cfdisk Command second partition is linux (ext4)

```bash
# format the created partition by ext4
sudo mkfs.ext4 -L rootfs loop<x>p2
```
Mount the partitions **boot** to directory boot and **rootfs** to directory rootfs
```bash
# mount the loop<x> device partition 1 to boot directory
mount /dev/loop<x>p1 boot

# mount the loop<x> device partition 2 to rootfs directory
mount /dev/loop<x>p2 rootfs
```



### Start Qemu

Start Qemu with the **Emulated SD** card

```bash
qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sdcard/sd.img
```

## TFTP Protocol

### Install and Configure TFTP

```bash
#Download tftp protocol
sudo apt-get install tftpd-hpa
#Change the configuration of tftp
vim /etc/default/tftpd-hpa
	#write inside the file
    tftf_option = “--secure –-create”
#Restart the protocal
Systemctl restart tftpd-hpa
#Make sure the tftp protocol is running
Systemctl status tftpd-hpa
#Change the file owner
cd /srv
chown tftp:tftp tftp 
#Move your image or file to the server
cp [File name] /srv/tftp
```

### Create tap Interfcae 
tap interface is a virtual Ethernet used for communication between qemu and host

Create a script `qemu-ifup`  to start tap interface

```bash
#!/bin/sh
ip a add 192.168.1.92/24 dev $1
ip link set $1 up
```

### Start Qemu

In order to start Qemu with the new virtual ethernet

```bash
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel u-boot/u-boot \
-sd sd.img \
-net tap,script=./qemu-ifup -net nic
```

### Setup qemu IP address

```bash
#Apply ip address for embedded device
setenv ipaddr [chose]
#Set the server ip address to tap interface ipaddr(192.168.1.92)
setenv serverip [host ip address]
```

#### WARNING ####
**The ip address should has the same net mask**


## Load File to RAM

First we need to know the ram address by running the following commend

```bash
# this commend will show all the board information and it start ram address
bdinfo
```

### Load from FAT (sdcard)

```bash
# addressRam is a variable knowen from bdinfo commend
fatload mmc 0:1 [addressRam] [fileName]
```

### Load from TFTP

```bash
# addressRam is a variable known from bdinfo commend
tftp [addressRam] [fileName]
```



## Script for U-boot
You can write a script for **U-boot** to set  some configurations desired, and then create a **uboot script image** file using **`mkimage tool`**

### Create u-boot script

Create `uboot-script` with the desired configurations and environment variables needed ...the script extension must be *.txt*
``` bash
setenv boottargs=console=tty console=ttyAMA0 root=/dev/mmcblk0p2 rootfstype=ext4 rw init=/sbin/init

setenv ipaddr 192.168.1.95
setenv serverip 192.168.1.92

fatload mmc 0:1 $kernel_addr_r zImage
fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb

bootz $kernel_addr_r - $fdt_addr_r
```
### U-boot Image
Convert the script into u-boot image.

```bash
mkimage -T script -C none -n "uboot script" -d ubootScript.txt ubootScript.img
```

- `-T`   Specify the File Type
- `-C` Specify the Compression Type
- `-n`  Image Name
- `-d` Input File --> then Output file

### Load U-boot Image
- Set an environment variable to a specific address **(DRAM Address)** to store `uboot image`
- Set bootcmd variable to load **uboot image** into the address specified before...then run the **script_addr** environment variable

```bash
setenv script_addr 0x60000000

setenv bootcmd fatload mmc 0:1 $uboot_scriptaddr ubootScript.img; source $script_addr
```

