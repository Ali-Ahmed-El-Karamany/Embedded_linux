#include "pcd_n.h"

/*  Driver data intialization   */
pcdrv_private_data pcdrv_data =
{
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = 
    {
        /*  First device intialization  */
        [0] = {
                .buffer = pcdev1_buffer,
                .size = PCDEV1_MEM_SIZE,
                .name = "device1",
                .perm = RDONLY
            },

        /*  Second device intialization  */
        [1] = {
                .buffer = pcdev2_buffer,
                .size = PCDEV2_MEM_SIZE,
                .name = "device2",
                .perm = WRONLY
            },

            /*  Third device intialization  */
        [2] = {
                .buffer = pcdev3_buffer,
                .size = PCDEV3_MEM_SIZE,
                .name = "device3",
                .perm = RDWR
            },

            /*  Fourth device intialization  */
        [3] = {
                .buffer = pcdev4_buffer,
                .size = PCDEV4_MEM_SIZE,
                .name = "device4",
                .perm = RDWR
            }   
    }
};

/********************************************************  FUNCTION DEFINITIONS    ********************************************************/


/*
 * check_Permission() - This function checks the permissions of the device file
 * @brief: This function is called when the file is opened which if device required permissions meets the user permissions
 * @param dev_perm: device permissions (RDONLY, WRONLY, RDWR)
 * @param acc_mode: user file access mode (FMODE_READ, FMODE_WRITE)
 * @return 0 on success, -EPERM on failure
 */
int check_permission(int dev_perm, int acc_mode)
{

	if(dev_perm == RDWR)
    {
        return 0;
    }
	
	// read only access
	if( (dev_perm == RDONLY) && ( (acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE) ) )
	{
        return 0;
    }
	
	// write only access
	if( (dev_perm == WRONLY) && ( (acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ) ) )
    {
        return 0;
    }

	return -EPERM;

}

/*
 * pcd_open() - This function shall be called when the file is opened
 * @brief: This function shall be called when the file is opened which if successful, will call either read() or write()
 * @param inode: pointer to the inode structure
 * @param p_file: pointer to the file structure
 * @return 0 on success, -EPERM on failure
 */
int pcd_open (struct inode *inode, struct file *p_file)
{
    int ret_val;
    int minor_n;
    pcdev_private_data *pcdev_data;

    /*  find which device file will be opened   */
    minor_n = MINOR(inode->i_rdev);
    pr_info("minor access: %d\n",minor_n);

    /*  get the device private data */
    pcdev_data = container_of(inode->i_cdev,pcdev_private_data,pcd_cdev);

    /*  save device private data in the file pointer for the other functions    */
    p_file->private_data = pcdev_data;

    /*  check the device permissions    */
    ret_val = check_permission(pcdev_data->perm, p_file->f_mode);
    
    if(ret_val)
    {
        pr_info("open was unsuccessfull\n");
        return ret_val;

    }

    pr_info("open was successfull\n");
    return ret_val;
}

/*
 * pcd_read() - This function shall be called when the file is read
 * @brief: This function shall be called when the file is read which if successful, will read the device buffer
 * @param p_file: pointer to the file structure
 * @param buff: pointer to the buffer
 * @param count: number of bytes to be read
 * @param f_pos: offset of the buffer
 * @return number of bytes read
 */
ssize_t pcd_read (struct file *p_file, char __user *buff, size_t count, loff_t *f_pos)
{
    /*  extract the device data*/
    pcdev_private_data *pcdev_data = (pcdev_private_data*)p_file->private_data;
    /*  get the device maximum memory size */
    int max_size = pcdev_data->size;
    pr_info("Read operation has been requested for %zu bytes\n", count);
    pr_info("current file position: %lld\n", *f_pos);

    /*  Adjust the the count if it is bigger than the device memory size    */
    if((*f_pos + count) > max_size)
    {
        count = pcdev_data->size - *f_pos;
    }

    /*  copy the data from the kernel space to the user space   */
    if(copy_to_user(buff, pcdev_data->buffer+(*f_pos), count))
    {
        return -EFAULT;
    }

    /* Update the current file position */
    *f_pos += count;

    pr_info("number of successfully read bytes: %zu\n",count);
    pr_info("updated file position: %lld\n", *f_pos);

    /*  Return the number of bytes successfully read    */
    return count;
}

/*
 * pcd_write() - This function shall be called when the file is to be written
 * @brief: This function shall be called when the file is to be written which if successful, will write to the device buffer
 * @param p_file: pointer to the file structure
 * @param buff: pointer to the buffer
 * @param count: number of bytes to be written
 * @param f_pos: offset of the buffer
 * @return number of bytes written
 */
ssize_t pcd_write (struct file *p_file, const char __user *buff, size_t count, loff_t *f_pos)
{

    /* extract the device data  */
    pcdev_private_data *pcdev_data = (pcdev_private_data*)p_file->private_data;

    /*  get the device maximum memory size  */
    int max_size = pcdev_data->size;

    pr_info("Write operation has been requested for %zu bytes\n",count);
    pr_info("current file position: %lld\n", *f_pos);

    /*  Adjust the the count if it is bigger than the device memory size    */
    if((*f_pos + count) > max_size)
    {
        count = max_size - *f_pos;
    }

    if(!count)
    {
        pr_err("no sapce left for Writting\n");
        return -ENOMEM;
    }

    /*  copy the data from the user space to the kernel space   */
    if(copy_from_user(pcdev_data->buffer+(*f_pos), buff, count))
    {
        return -EFAULT;
    }

    /* Update the current file position */
    *f_pos += count;

    pr_info("number of successfully written bytes: %zu\n",count);
    pr_info("updated file position: %lld\n", *f_pos);

    /*  Return the number of bytes successfully written    */
    return count;
}


/*
 * pcd_release() - This function shall be called when the file is closed
 * @brief: This function shall be called when the file is closed which if successful, will close the file
 * @param inode: pointer to the inode structure
 * @param p_file: pointer to the file structure
 * @return 0 on success
 */
int pcd_release (struct inode *inode, struct file *p_file)
{
    pr_info("release successfully\n");
    return 0;
}



/******************************************  File operations of the driver   *********************************************************************/
static struct file_operations pcd_fops = {
    .open = pcd_open,
    .release = pcd_release,
    .read = pcd_read,
    .write = pcd_write,
    .owner = THIS_MODULE
};

/****************************************     This is the init function that is called when the module is loaded    ********************************/
static int __init pcd_init(void)
{
    int ret_val = 0;
    int i;

     /************************************     1- Dynamically allocate and register a device number      ******************************************/

    // device_number is a capsulated number that contains the major and minor numbers
    // 0 --> base number (to start from)
    // NO_OF_DEVICES --> count of number of devices
    ret_val = alloc_chrdev_region(&pcdrv_data.device_number, 0, NO_OF_DEVICES, "pcd_devices");

    if(ret_val < 0)
    {
        pr_err("ERROR!...allocating device number\n");
        goto out;
    }

    /*************************************     2- Create the device class under /sys/class        *************************************************/
        
    pcdrv_data.pcd_class = class_create("pcd_class");

    if (pcdrv_data.pcd_class == NULL)
    {
        pr_err("ERROR!...failed to create device class\n");
        goto unreg_char_dev;
    }
    
    for(i=0;i<NO_OF_DEVICES;i++)
    {
        pr_info("Device number <MAJOR>:<MINOR> = %d:%d\n", MAJOR(pcdrv_data.device_number+i), MINOR(pcdrv_data.device_number+i));
            
    
        /*************************************     3- Initialize the cdev structure with the file operations      *************************************/

        cdev_init(&pcdrv_data.pcdev_data[i].pcd_cdev,&pcd_fops);
        pcdrv_data.pcdev_data[i].pcd_cdev.owner = THIS_MODULE;


        /*************************************     4- Register the device (cdev structure) to the kernel       ****************************************/
        
        ret_val = cdev_add(&pcdrv_data.pcdev_data[i].pcd_cdev, pcdrv_data.device_number+i, 1);
        if(ret_val != 0)
        {
            pr_err("ERROR!...failed to register the device to the kernel\n");
            goto cdev_del;
        }

        /*************************************     5- populate sysfs with device information        *************************************************/
        
        pcdrv_data.pcd_device = device_create(pcdrv_data.pcd_class, NULL, pcdrv_data.device_number+i, NULL, "pcdev-%d",i);

        if (pcdrv_data.pcd_device == NULL)
        {
            pr_err("ERROR!...failed to create device. \n");
            goto class_del;
        }
    }
    pr_info("Module init SUCCEEDED\n");
    return 0;

cdev_del:
class_del:
        for(;i>=0;i--)
        {
            device_destroy(pcdrv_data.pcd_class, pcdrv_data.device_number+i);
            cdev_del(&pcdrv_data.pcdev_data[i].pcd_cdev);
        }
        class_destroy(pcdrv_data.pcd_class);
unreg_char_dev:
        unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);
out:
    pr_info("Module insertion failed");
    return ret_val;

}

// This is the exit function that is called when the module is removed
static void __exit pcd_exit(void)
{
    int i;

    for(i = 0;i<NO_OF_DEVICES;i++)
    {
        // Remove the devices from /dev
        device_destroy(pcdrv_data.pcd_class, pcdrv_data.device_number+i);
        // Delete the device from the system
        cdev_del(&pcdrv_data.pcdev_data[i].pcd_cdev);
    }

    // Remove the class from /sys/class
    class_destroy(pcdrv_data.pcd_class);

    // Unregister the device number (remove)
    unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);
    pr_info("Module unloaded... bye bye!\n");
}

// These are the module init and exit functions
module_init(pcd_init);
module_exit(pcd_exit);

// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aly");
MODULE_DESCRIPTION("pseudo device driver");