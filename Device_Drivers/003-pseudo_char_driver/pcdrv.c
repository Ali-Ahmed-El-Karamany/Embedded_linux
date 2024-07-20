#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>


#define DEV_MEM_SIZE 512


// pseudo Device memory
static unsigned char device_buffer[DEV_MEM_SIZE];

/*   holds the deviced number   */
dev_t device_number;

/*  cdv varibale    */
static struct cdev pcd_cdev;

/*  a pointer holds the return of the function class_create */
struct class *pcd_class;

struct device *pcd_device;


loff_t pcd_lseek (struct file *p_file, loff_t offset, int whence)
{
    loff_t temp;
    pr_info("lseek operation has been requested\n");

    switch (whence)
    {
    case SEEK_SET:
        if((offset > DEV_MEM_SIZE) || (offset < 0))
        {
            return -EINVAL;
        }
        p_file->f_pos = offset;
        break;
    
    case SEEK_CUR:
        temp = p_file->f_pos + offset;
        if((temp > DEV_MEM_SIZE) || (temp < 0))
        {
            return -EINVAL;
        } 
        p_file->f_pos = temp;
        break;

    case SEEK_END:
        temp = DEV_MEM_SIZE + offset;
        if((temp > DEV_MEM_SIZE) || (temp < 0))
        {
            return -EINVAL;
        } 
        p_file->f_pos = temp;
        break;
    
    default:
        return EINVAL;
    }

    return p_file->f_pos;    
}

ssize_t pcd_read (struct file *p_file, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Read operation has been requested for %zu bytes\n", count);
    pr_info("current file position: %lld\n", *f_pos);

    /*  Adjust the the count if it is bigger than the device memory size    */
    if((*f_pos + count) > DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE - *f_pos;
    }

    /*  copy the data from the kernel space to the user space   */
    if(copy_to_user(buff, &device_buffer[*f_pos], count))
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

ssize_t pcd_write (struct file *p_file, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Write operation has been requested for %zu bytes\n",count);
    pr_info("current file position: %lld\n", *f_pos);

    /*  Adjust the the count if it is bigger than the device memory size    */
    if((*f_pos + count) > DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE - *f_pos;
    }

    if(!count)
    {
        pr_err("no sapce left for Writting\n");
        return -ENOMEM;
    }

    /*  copy the data from the user space to the kernel space   */
    if(copy_from_user(&device_buffer[*f_pos], buff, count))
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
int pcd_open (struct inode *inode, struct file *p_file)
{
    pr_info("open successfully\n");
    return 0;
}
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

     /************************************     1- Dynamically allocate and register a device number      ******************************************/

    // device_number is a capsulated number that contains the major and minor numbers
    // 0 --> base number (to start from)
    // 1 --> count of number of devices
    ret_val = alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");

    if(ret_val < 0)
    {
        pr_err("ERROR!...allocating device number\n");
        return -1;
    }
    else
    {
        pr_info("Device number <MAJOR>:<MINOR> = %d:%d\n", MAJOR(device_number), MINOR(device_number));
    }


    /*************************************     2- Initialize the cdev structure with the file operations      *************************************/

    cdev_init(&pcd_cdev,&pcd_fops);
    pcd_cdev.owner = THIS_MODULE;


    /*************************************     3- Register the device (cdev structure) to the kernel       ****************************************/
    
    // 1 --> number of devices
    ret_val = cdev_add(&pcd_cdev, device_number, 1);
    if(ret_val != 0)
    {
        unregister_chrdev_region(device_number,1);
        pr_err("ERROR!...failed to register the device to the kernel\n");
        return -1;
    }
    

    /*************************************     4- Create the device class under /sys/class        *************************************************/
    
    pcd_class = class_create("pcd_class");

    if (pcd_class == NULL)
    {
        pr_err("ERROR!...failed to create device class\n");
        cdev_del(&pcd_cdev);
        unregister_chrdev_region(device_number,1);

        return -1;
    }

    /*************************************     5- populate sysfs with device information        *************************************************/
    
    pcd_device = device_create(pcd_class, NULL, device_number, NULL, "pcd");

    if (pcd_device == NULL)
    {
        pr_err("ERROR!...failed to create device. \n");
        class_destroy(pcd_class);
        cdev_del(&pcd_cdev);
        unregister_chrdev_region(device_number,1);

        return -1;
    }

    pr_info("Module init SUCCEEDED\n");
    return 0;
}

// This is the exit function that is called when the module is removed
static void __exit pcd_exit(void)
{
    // Remove the device from /dev
    device_destroy(pcd_class, device_number); // Remove the device from /dev

    // Remove the class from /sys/class
    class_destroy(pcd_class); // Remove

    // Delete the device from the system
    cdev_del(&pcd_cdev);

    // Unregister the device number (remove)
    unregister_chrdev_region(device_number, 1);
    pr_info("Module unloaded... bye bye!\n");
}

// These are the module init and exit functions
module_init(pcd_init);
module_exit(pcd_exit);

// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aly");
MODULE_DESCRIPTION("pseudo device driver");