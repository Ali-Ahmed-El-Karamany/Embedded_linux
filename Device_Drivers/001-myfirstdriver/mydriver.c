#include<linux/module.h>
#include<linux/init.h>

// init function for kernel
// static --> as the init function can not be used from another drivers
// __init to put the function in the "init Section" so after the function execution it will be deleted from RAM
static int  __init mydriver_init(void)
{
    printk("Hello!.. from my first device driver \n");

    return 0;
}

// exit function to delete the function from RAM
// static --> as the init function can not be used from another drivers
// __exit used to delete the "exit Section" from the RAM in dyanmic modules
static void __exit mydriver_exit(void)
{
    printk("bye..bye \n");
}



module_init(mydriver_init);
module_exit(mydriver_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My First Driver");
MODULE_AUTHOR("Aly Ahmed");