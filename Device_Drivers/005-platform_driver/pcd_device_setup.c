#include "platform.h"
/****************************************     DEVICE RELEASE FUNCTION    ***************************************/
void pcdev_release(struct device *dev)
{
    pr_info("Device realeased\n");
}

/****************************************     PLATFORM DEVICE SPECIFIC DATA    ********************************/
pcdev_platform_data pcdev_data[NO_OF_DEVICES] = {
    [AXZ] = {
        .size = PCDEV1_MEM_SIZE,
        .perm = RDWR
    },

    [BXZ] = {
        .size = PCDEV2_MEM_SIZE,
        .perm = RDWR
    },

    [CXZ] = {
        .size = PCDEV3_MEM_SIZE,
        .perm = RDONLY
    },

    [DXZ] = {
        .size = PCDEV4_MEM_SIZE,
        .perm = WRONLY
    }
};

/****************************************     PLATFORM DEVICES    *****************************************/
struct platform_device platform_pcdev1 = {
    .name = "pcdev_AXZ",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_data[0],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev2 = {
    .name = "pcdev_BXZ",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_data[1],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev3 = {
    .name = "pcdev_CXZ",
    .id = 2,
    .dev = {
        .platform_data = &pcdev_data[2],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev4 = {
    .name = "pcdev_DXZ",
    .id = 3,
    .dev = {
        .platform_data = &pcdev_data[3],
        .release = pcdev_release
    }
};

/****************************************     This is the init function that is called when the module is loaded    ********************************/
static int __init pcdev_platform_init(void)
{
    //Register platform devices
    platform_device_register(&platform_pcdev1);
    platform_device_register(&platform_pcdev2);
    platform_device_register(&platform_pcdev3);
    platform_device_register(&platform_pcdev4);

    pr_info("Device Setup module Loaded\n");


    return 0;
}

/****************************************     This is the exit function that is called when the module is removed    ********************************/
static void __exit pcdev_platform_exit(void)
{
    // Unregister platform devices
    platform_device_unregister(&platform_pcdev1);
    platform_device_unregister(&platform_pcdev2);
    platform_device_unregister(&platform_pcdev3);
    platform_device_unregister(&platform_pcdev4);

    pr_info("Device Setup module unloaded\n");
}

// These are the module init and exit functions
module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aly");
MODULE_DESCRIPTION("Module to register platform devices");