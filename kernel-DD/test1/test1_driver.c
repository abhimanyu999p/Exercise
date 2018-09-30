#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>

int test_open (struct inode *pinode, struct file *pfile)
{
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);
   return 0;
}
ssize_t test_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);
   return 0;
}
ssize_t test_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);
   return length;
}
int test_close (struct inode *pinode, struct file *pfile)
{
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);
   return 0;
}

/* to hold file operations performed by this device*/
struct file_operations test1_driver_file_operations = {
   .owner   = THIS_MODULE,
   .open    = test_open,
   .read    = test_read,
   .write   = test_write,
   .release = test_close,
};

int test1_driver_function_init(void)
{
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);

   /* Register with kernel n indicate that a char device is being registered*/
   register_chrdev(256 /*major number*/,
                   "test1_char_device" /*name of the driver*/,
                   &test1_driver_file_operations /*file operations*/);

   return 0;
}

void test1_driver_function_exit(void)
{
   /* unregister the char device*/
   unregister_chrdev(256,"test1_char_device");
   printk(KERN_ALERT "Inside function %s\n",__FUNCTION__);
}

module_init(test1_driver_function_init);
module_exit(test1_driver_function_exit);
