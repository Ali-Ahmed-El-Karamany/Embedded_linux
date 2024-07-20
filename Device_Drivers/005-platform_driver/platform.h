#ifndef PLATFORM_H
#define PLATFORM_H
/*****************************************  INCLUDES    ***************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/mod_devicetable.h>
/*****************************************  DEFINITIONS    ************************************************/

/* TOTAL NUMBER OF DEVICES  */
#define NO_OF_DEVICES 4

/*  DEVICES MEMORY  */
#define PCDEV1_MEM_SIZE 512
#define PCDEV2_MEM_SIZE 128
#define PCDEV3_MEM_SIZE 64
#define PCDEV4_MEM_SIZE 32

/*  PERMISSIONS */
#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11

/**************************************  PLATFORM DEVICE DATA STRUCTURE    *********************************/
typedef struct 
{
    int size;       // Holds the device memory size
    int perm;       // Holds the device permission
}pcdev_platform_data;

/***************************************  DEVICE PRIVATE DATA STRUCTURE    *********************************/
typedef struct 
{
    pcdev_platform_data pdata;      // platform device specific data
    char *buffer;                   // Holds the device buffer
    dev_t dev_number;               // Device number
    struct cdev pcd_cdev;           // Device cdev structure
}pcdev_private_data;

/****************************************  DRIVER PRIVATE DATA STRUCTURE    *********************************/
typedef struct 
{
    int total_devices;              // Total number of probed devices 
    dev_t device_num_base;          // Holds the base of the device Number
    struct class *pcd_class;
    struct device *pcd_device;    
}pcdrv_private_data;

/*****************************************  TYPE DEFINITIONS FOR THE DEVICES    ******************************/
enum pcdev_names {
    AXZ ,
    BXZ,
    CXZ,
    DXZ
};
/******************************************  FUNCTION PROTOTYPES    *******************************************/

/*
 * check_Permission() - This function checks the permissions of the device file
 * @brief: This function is called when the file is opened which if device required permissions meets the user permissions
 * @param dev_perm: device permissions (RDONLY, WRONLY, RDWR)
 * @param acc_mode: user file access mode (FMODE_READ, FMODE_WRITE)
 * @return 0 on success, -EPERM on failure
 */
int check_permission(int dev_perm, int acc_mode);

/*
 * pcd_open() - This function shall be called when the file is opened
 * @brief: This function shall be called when the file is opened which if successful, will call either read() or write()
 * @param inode: pointer to the inode structure
 * @param p_file: pointer to the file structure
 * @return 0 on success, -EPERM on failure
 */
int pcd_open (struct inode *inode, struct file *p_file);

/*
 * pcd_read() - This function shall be called when the file is read
 * @brief: This function shall be called when the file is read which if successful, will read the device buffer
 * @param p_file: pointer to the file structure
 * @param buff: pointer to the buffer
 * @param count: number of bytes to be read
 * @param f_pos: offset of the buffer
 * @return number of bytes read
 */
ssize_t pcd_read (struct file *p_file, char __user *buff, size_t count, loff_t *f_pos);

/*
 * pcd_write() - This function shall be called when the file is to be written
 * @brief: This function shall be called when the file is to be written which if successful, will write to the device buffer
 * @param p_file: pointer to the file structure
 * @param buff: pointer to the buffer
 * @param count: number of bytes to be written
 * @param f_pos: offset of the buffer
 * @return number of bytes written
 */
ssize_t pcd_write (struct file *p_file, const char __user *buff, size_t count, loff_t *f_pos);

/*
 * pcd_release() - This function shall be called when the file is closed
 * @brief: This function shall be called when the file is closed which if successful, will close the file
 * @param inode: pointer to the inode structure
 * @param p_file: pointer to the file structure
 * @return 0 on success
 */
int pcd_release (struct inode *inode, struct file *p_file);

/*
 * pcdrv_probe() - This function shall be called when the device is probed
 * @brief: This function shall be called when the device is probed which if successful, will probe the device
 * @param plat_dev: pointer to the matched platform device 
 * @return 0 on success
 */
int pcdrv_platform_probe(struct platform_device *plat_dev);

/*
 * pcdrv_remove() - This function shall be called when the device is removed
 * @brief: This function shall be called when the device is removed which if successful, will remove the device
 * @param plat_dev: pointer to the matched platform device 
 * @return 0 on success
 */
int pcdrv_platform_remove(struct platform_device *plat_dev);


#endif