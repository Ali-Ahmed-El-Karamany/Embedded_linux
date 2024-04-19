# Device Driver
Simple virtual device driver in kernel space to take a section from the memory. Use this section to print a messege.


## Implementation

### driver.c 

```c
#include<linux/module.h>
#include<linux/init.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My First Driver");
MODULE_AUTHOR("Aly Ahmed");

static int  __init mydriver_init(void)
{
    printk("Hello!.. from my first device driver \n");

    return 0;
}


static void __exit mydriver_exit(void)
{
    printk("bye..bye \n");
}



module_init(mydriver_init);
module_exit(mydriver_exit);
```
`printk()` The print function used in kernel space.


`module_init();` Kernel looks for **module_init** as the entry point, executing the specified function at the start of the program.
module_exit();

`module_exit()` It is the exit point for kernel space.

`static void __init` to put the function **mydriver_init** in **.init** section when it is invoked.  **__init** used to remove the function from the memory after the execution. 

`static void __exit` to put the function **mydriver_exit** in **.exit** section when it is invoked.  **__exit** used to remove the function from the memory after the execution to save resources.

### Makefile
To build the kernel module we should use a Make file.

```Make
obj-m=mydriver.o
BDIR=/lib/modules/$(shell uname -r)/build

all:
	make -C ${BDIR} M=${PWD} modules

clean:
	make -C ${BDIR} M=${PWD} clean
```

`obj-m` Used to build the module Dynamically.

`BDIR` Variable to hold the path of the kerlnel build directory.

`uname -r` refers to the version of my device linux.

`make -C ${BDIR} M=${PWD} modules` call the **Makefile** of the kernel.

## Compilation

to compile the module just run make

```bash
make
```

-After the compilation the kernel module `mydriver.ko` will appear.



## execution

`insmod` To insert the module to the kernel 
  ```bash
  sudo insmod mydriver.ko
  ```
  `dmesg` command to see the messages from the kernel 
```bash
dmesg
```
it will print **`Hello!.. from my first device driver`**

`rmmod` To remove the module from the kernel 
```bash
sudo rmmod mydriver
```

when use `dmesg` command it will print **`bye..bye`** 