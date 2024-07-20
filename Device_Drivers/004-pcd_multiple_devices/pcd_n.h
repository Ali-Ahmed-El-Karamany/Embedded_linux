#ifndef PCD_N_H
#define PCD_N_H

/*****************************************  INCLUDES    **********************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>

/*****************************************  DEFINITIONS    **********************************************/

#define PCDEV1_MEM_SIZE 1024
#define PCDEV2_MEM_SIZE 512
#define PCDEV3_MEM_SIZE 512
#define PCDEV4_MEM_SIZE 1024

/* TOTAL NUMBER OF DEVICES  */
#define NO_OF_DEVICES   4

/*  PERMISSIONS*/

#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11
/*****************************************  DEVICES MEMORY    **********************************************/


static unsigned char pcdev1_buffer[PCDEV1_MEM_SIZE];
static unsigned char pcdev2_buffer[PCDEV2_MEM_SIZE];
static unsigned char pcdev3_buffer[PCDEV3_MEM_SIZE];
static unsigned char pcdev4_buffer[PCDEV4_MEM_SIZE];

/*****************************************      **********************************************/


/*****************************************  DEVICE PRIVATE DATA STRUCTURE    **********************************************/

typedef struct
{
    const char *name;               // Holds the device name
    char *buffer;                   // Holds the device buffer
    int size;                       // Holds the device memory size
    int perm;                       // Holds the device permission
    struct cdev pcd_cdev;           // Device cdev structure
}pcdev_private_data;



/*****************************************  DRIVER PRIVATE DATA STRUCTURE    **********************************************/
typedef struct 
{
    int total_devices;                              // Total number of devices
    dev_t device_number;                            // Device number
    struct class *pcd_class;
    struct device *pcd_device;
    pcdev_private_data pcdev_data[NO_OF_DEVICES];    
    
}pcdrv_private_data;


/********************************************************  FUNCTION PROTOTYPES    ********************************************************/

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

#endif