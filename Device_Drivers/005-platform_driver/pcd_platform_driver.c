#include "platform.h"


pcdrv_private_data pcdrv_data;

/******************************************  DRIVER FILE OPERATIONS   *********************************************************************/
static struct file_operations pcd_fops = {
    .open = pcd_open,
    .release = pcd_release,
    .read = pcd_read,
    .write = pcd_write,
    .owner = THIS_MODULE
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
    return 0;
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
    return 0;
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
    return -ENOMEM;
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

/*
 * pcdrv_probe() - This function shall be called when the device is probed
 * @brief: This function shall be called when the device is probed which if successful, will probe the device
 * @param plat_dev: pointer to the matched platform device 
 * @return 0 on success
 */
int pcdrv_platform_probe(struct platform_device *plat_dev)
{
    int ret_val = 0;
    /* pointer to access device private data structure */
    pcdev_private_data *pcdev_data;

    /*  pointer to platform device data */
    pcdev_platform_data *pdata;

    pr_info("A device is detected\n");

    /****************************************     Get the platform device data    ********************************/
    pdata = plat_dev->dev.platform_data;

    if(!pdata)
    {
        pr_info("No platform data available\n");
        ret_val = -EINVAL;
        goto out;
    }

    /****************************************     Dynamically allocate memory for the device private data    ********************************/
    pcdev_data = kzalloc(sizeof(pcdev_private_data),GFP_KERNEL);
    if(!pcdev_data)
    {
        pr_info("cannot allocate memory the device private data\n");
        ret_val = -ENOMEM;
        goto out;
    }

    /*  Assign the platform data into the device private data   */
    pcdev_data->pdata = *pdata;

    /*  Save the device private data in the platform device structure   */
    dev_set_drvdata(&plat_dev->dev, pcdev_data);

    pr_info("Device size = %d\n",pcdev_data->pdata.size);
    pr_info("Device permission = %d\n",pcdev_data->pdata.perm);
    
    /****************************************     Dynamically allocate memory for the device buffer    *******************************************/
    pcdev_data->buffer = kzalloc(pcdev_data->pdata.size,GFP_KERNEL);
    if(!pcdev_data)
    {
        pr_info("cannot allocate memory the device buffer\n");
        ret_val = -ENOMEM;
        goto pcdev_data_free;
    }
    
    /****************************************     Get the device number    ***********************************************************************/
    pcdev_data->dev_number = pcdrv_data.device_num_base + plat_dev->id;

    /****************************************     Initialize the cdev structure with the file operations      ************************************/
    cdev_init(&pcdev_data->pcd_cdev,&pcd_fops);
    pcdev_data->pcd_cdev.owner = THIS_MODULE;
    
    /****************************************     Register the device (cdev structure) to the kernel       ***************************************/
    ret_val = cdev_add(&pcdev_data->pcd_cdev, pcdev_data->dev_number, 1);
    if(ret_val != 0)
    {
        pr_err("ERROR!...failed to register the device cdev structure to the kernel\n");
        goto free_buffer;
    }

    /****************************************     Populate sysfs with device information        **************************************************/
    pcdrv_data.pcd_device = device_create(pcdrv_data.pcd_class, NULL, pcdev_data->dev_number, NULL, "pcdev-%d",plat_dev->id);
    
    if (pcdrv_data.pcd_device == NULL)
    {
        pr_err("ERROR!...failed to create device. \n");
        goto cdev_del;
    }

    /* Increment the count of probed devices   */
    pcdrv_data.total_devices++;

    pr_info("The probe is successful\n");
    return 0;

/*  ERROR HANDLING  */
cdev_del:
    cdev_del(&pcdev_data->pcd_cdev);
free_buffer:
    kfree(pcdev_data->buffer);
pcdev_data_free:
    kfree(pcdev_data);
out:
    pr_info("Device probe failed\n");
    return ret_val;
}

/*
 * pcdrv_remove() - This function shall be called when the device is removed
 * @brief: This function shall be called when the device is removed which if successful, will remove the device
 * @param plat_dev: pointer to the matched platform device 
 * @return 0 on success
 */
int pcdrv_platform_remove(struct platform_device *plat_dev)
{
    pcdev_private_data *pcdev_data = dev_get_drvdata(&plat_dev->dev);

    /*  Remove the device was created   */
    device_destroy(pcdrv_data.pcd_class, pcdev_data->dev_number);

    /*  Delete the device cdev structure from the system   */
    cdev_del(&pcdev_data->pcd_cdev);

    /*  Free the allocated memory for the device data and buffer    */
    kfree(pcdev_data->buffer);
    kfree(pcdev_data);

    /* Decrement the count of probed devices   */
    pcdrv_data.total_devices++;

    pr_info("A device is removed\n");
    return 0;
}

struct platform_device_id pcdev_ids[] = {
    
    [AXZ] = {.name = "pcdev_AXZ", .driver_data = AXZ},
    [BXZ] = {.name = "pcdev_BXZ", .driver_data = BXZ},
    [CXZ] = {.name = "pcdev_CXZ", .driver_data = CXZ},
    [DXZ] = {.name = "pcdev_DXZ", .driver_data = DXZ},
    /*  NULL Terminated entry   */
    {}
};

/******************************************  PLATFORM DRIVER OPERATIONS    ************************************************************************/
struct platform_driver pcdrv_platform = {
    .probe = pcdrv_platform_probe,
    .remove = pcdrv_platform_remove,
    .id_table = pcdev_ids,
    .driver = {
        .name = "char_device"
    }
};

/****************************************     This is the init function that is called when the module is loaded    ********************************/
static int __init pcdrv_platform_init(void)
{
    int ret_val = 0;

    /************************************     Dynamically allocate a device number for the devices     *********************************************/
    ret_val = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, NO_OF_DEVICES, "pcd_devices");
    if(ret_val < 0)
    {
        pr_err("ERROR!...allocating device number\n");
        return ret_val;
    }

    /*************************************     Create the device class under /sys/class        ****************************************************/
    pcdrv_data.pcd_class = class_create("pcd_class");

    if (pcdrv_data.pcd_class == NULL)
    {
        pr_err("ERROR!...failed to create device class\n");
        ret_val = PTR_ERR(pcdrv_data.pcd_class);
        unregister_chrdev_region(pcdrv_data.device_num_base,NO_OF_DEVICES);
        return ret_val;
    }

    /*************************************     Register a platform driver        ******************************************************************/
    ret_val = platform_driver_register(&pcdrv_platform);

    if(ret_val < 0)
    {
        pr_err("ERROR!...Failed to Register platform driver\n");
        return ret_val;
    }
    pr_info("pcd platform driver is loaded\n");
    return 0;
}

// This is the exit function that is called when the module is removed
static void __exit pcdrv_platform_exit(void)
{
    /*************************************     Unregister a platform driver        ******************************************************************/
    platform_driver_unregister(&pcdrv_platform);
    
    /*************************************     class Destroy        *********************************************************************************/
    class_destroy(pcdrv_data.pcd_class);

    /*************************************     Unregister device number for the devices        *****************************************************/
    unregister_chrdev_region(pcdrv_data.device_num_base,NO_OF_DEVICES);

    pr_info("pcd platform driver is unloaded\n");
}

// These are the module init and exit functions
module_init(pcdrv_platform_init);
module_exit(pcdrv_platform_exit);

// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aly");
MODULE_DESCRIPTION("pseudo charchter platform driver which handels platform devices");