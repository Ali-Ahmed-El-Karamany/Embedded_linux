#include<linux/module.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#define SIZE 255

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("proc file");
MODULE_AUTHOR("Aly");


struct proc_dir_entry *myproc_ret = NULL;
static unsigned char myBuffer[255] = "";

// This function is called when the file is read (read operation)
ssize_t myproc_read(struct file *myFileName, char __user *user_buffer, size_t count, loff_t *offs)
{
    // variable to save the not copied data returned from copy_to_user
    int not_copied;
    
    if((count + *offs) > SIZE)
    {
        count = SIZE - *offs;
    }

    // Copy the string to the user space (to terminal)
    not_copied = copy_to_user(user_buffer, &myBuffer[*offs], count); 
    if(not_copied != 0)
    {
        return -1;
    }
    *offs = count;
    printk("my proc file has been read.\n");
    
    return count;
}

// This function is called when the file is written (write operation)
ssize_t myproc_write(struct file *myFileName, const char __user *user_buffer, size_t count, loff_t *offs)
{   
    // variable to save the not copied data returned from copy_to_user
    int not_copied;
    
    if((count + *offs) > SIZE)
    {
        count = SIZE - *offs;
    }

    if(!count)
    {
        printk("no space left.\n");
        return -1;
    }
    // Copy the string from the user space (from terminal)
    not_copied = copy_from_user(&myBuffer[*offs], user_buffer, count);
    if(not_copied != 0)
    {
        return -1;
    }
    *offs = count; 
    printk("my proc file has been written.\n");
    return count;
}



struct proc_ops myproc_st =
{
    .proc_read = myproc_read,
    .proc_write = myproc_write
};



// init function called when the module loadded
static int  __init myproc_init(void)
{

    // Create a new new file in /proc
    // 0660 is the permission (rw- rw- ---)
    // NULL is the parent directory
    // fileoperation is the file operation structure
    myproc_ret = proc_create("Aly", 0666, NULL, &myproc_st);

    printk("hello...my module loaded! \n");
    return 0;
}

// exit function called when the module removed
static void  __exit myproc_exit(void)
{
    // removes the created file 
    proc_remove(myproc_ret);
    printk("bye bye..my module has removed! \n");

}

// Module init and exit functions
module_init(myproc_init);
module_exit(myproc_exit);


