/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<asm/io.h>

#include"myGpio.h"

#define GPFSEL0_OFFSET       0x00
#define GPFSEL1_OFFSET       0x04
#define GPFSEL2_OFFSET       0x08
#define GPFSEL3_OFFSET       0x0C
#define GPFSEL4_OFFSET       0x10
#define GPFSEL5_OFFSET       0x14
#define GPSET0_OFFSET        0x1C
#define GPSET1_OFFSET        0x20
#define GPCLR0_OFFSET        0x28
#define GPCLR1_OFFSET        0x2C
#define GPLEV0_OFFSET        0x34
#define GPLEV1_OFFSET        0x38
#define GPPUD_OFFSET         0x94
#define GPPUD_CLK0_OFFSET    0x98

#define DEVICE_NAME     "myGpio"
#define GPIO_BASE     0x3F200000
#define RANGE               0xA0
#define FLD_MASK            0x07

static int s32Major;
static void __iomem *vAddrBase;
static int dev_open = 0;

static inline int myGpio_FunctionSelect(Pin_t p)
{
  switch(p)
  {
    case 0 ... 9:
      return GPFSEL0_OFFSET;
    case 10 ... 19:
      return GPFSEL1_OFFSET;
    case 20 ... 29:
      return GPFSEL2_OFFSET;
    case 30 ... 39:
      return GPFSEL3_OFFSET;
    case 40 ... 49:
      return GPFSEL4_OFFSET;
    case 50 ... 53:
      return GPFSEL5_OFFSET;
    default:
      return -1;
  }
}

int myGpio_IoctlSetPinFunc(Pin_t pin, Pin_func_t func)
{
  u32 r1,r2;

  if(0 > (r1 = myGpio_FunctionSelect(pin)))
  {
    return -1;
  }
  else
  {
    r2 = ioread32((u32 *)(vAddrBase + r1));
    r2 &= ~(FLD_MASK << (pin%10)*3);
    r2 |= (func << (pin%10)*3);
    iowrite32(r2,(u32 *)(vAddrBase + r1));
    return 0;
  }
}

int myGpio_IoctlReadPin(Pin_t pin, Pin_mode_t *mode)
{
  u32 r1;

  switch (pin)
  {
    case 0 ... 31:
      r1 = ioread32((u32 *)(vAddrBase + GPLEV0_OFFSET));
      *mode = 1 & (r1 >> pin);
      break;
    case 32 ... 53:
      r1 = ioread32((u32 *)(vAddrBase + GPLEV1_OFFSET));
      *mode = 1 & (r1 >> (pin%32));
      break;
    default:
      return -1;
  }
 return 0;
}

int myGpio_IoctlWritePin(Pin_t pin, Pin_mode_t mode)
{
  switch (pin)
  {
    case 0 ... 31:
      if(mode == HIGH)
        iowrite32((1 << pin),(u32 *)(vAddrBase + GPSET0_OFFSET));
      else if(mode == LOW)
        iowrite32((1 << pin),(u32 *)(vAddrBase + GPCLR0_OFFSET));
      else
        return -1;
      break;
    case 32 ... 53:
      if(mode == HIGH)
        iowrite32((1 << pin%32),(u32 *)(vAddrBase + GPSET1_OFFSET));
      else if(mode == LOW)
        iowrite32((1 << pin%32),(u32 *)(vAddrBase + GPCLR1_OFFSET));
      else
        return -1;
      break;
    default:
      return -1;
  }
  return 0;
}

static int myGpio_open (struct inode *pinode, struct file *pfile)
{
  if(dev_open)
    return -EBUSY;

  dev_open++;

  printk(KERN_INFO "[myGpio]: %s\n",__FUNCTION__);
  return 0;
}

static ssize_t myGpio_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
  printk(KERN_INFO "[myGpio]: %s\n",__FUNCTION__);
  return 0;
}

static ssize_t myGpio_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
  printk(KERN_INFO "[myGpio]: %s\n",__FUNCTION__);
  return length;
}

static int myGpio_close (struct inode *pinode, struct file *pfile)
{
  dev_open--;

  printk(KERN_INFO "[myGpio]: %s\n",__FUNCTION__);
  return 0;
}

static long myGpio_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  myGpio_t p;
  int ret;

  if(copy_from_user(&p,(myGpio_t *)arg,sizeof(myGpio_t)))
  {
    return -EACCES;
  }

  switch(cmd)
  {
    case IOCTL_SET_PIN_FUNC:
      if( 0 > myGpio_IoctlSetPinFunc(p.pin,p.func))
      {
        ret = -1;
        printk(KERN_ALERT "IOCTLF_SET_PIN_FUNC failed\n");
      }
      break;

    case IOCTL_READ_PIN:
      if(0 > myGpio_IoctlReadPin(p.pin,&p.mode))
      {
        ret = -1;
        printk(KERN_ALERT "IOCTL_READ_PIN failed\n");
      }
      else if(copy_to_user((myGpio_t *)arg,&p,sizeof(myGpio_t)))
      {
        ret = -EACCES;
      }
      break;
    case IOCTL_WRITE_PIN:
      if(0 > myGpio_IoctlWritePin(p.pin,p.mode))
      {
        ret = -1;
        printk(KERN_ALERT "IOCTL_WRITE_PIN failed\n");
      }
      break;
    default:
      return -EINVAL;
  }
  return 0;
}

static struct file_operations myGpio_file_operations = {
    .owner   = THIS_MODULE,
    .open    = myGpio_open,
    .release = myGpio_close,
    .read    = myGpio_read,
    .write   = myGpio_write,
    .unlocked_ioctl = myGpio_ioctl
};

static int __init myGpio_driver_init(void)
{
  s32Major = register_chrdev(0,DEVICE_NAME, &myGpio_file_operations);

  if(0 > s32Major)
  {
    printk(KERN_ALERT "[myGpio]:Cannot register char device\n");
    return s32Major;
  }
  else if(NULL == (vAddrBase = ioremap(GPIO_BASE,RANGE)))
  {
    printk(KERN_ALERT "[myGpio]: ioremap fail\n");
    unregister_chrdev(s32Major, DEVICE_NAME);
    return -1;
  }
  printk(KERN_INFO "[myGpio]:Device registered successfully,major=%d\n",s32Major);

  return 0;
}

static void __exit myGpio_driver_exit(void)
{
  iounmap(vAddrBase);
  unregister_chrdev(s32Major, DEVICE_NAME);

  printk(KERN_INFO "[myGpio]:Device unregistered\n");
}

module_init(myGpio_driver_init);
module_exit(myGpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhimanyu Pandit");
MODULE_DESCRIPTION("A simple gpio driver");
