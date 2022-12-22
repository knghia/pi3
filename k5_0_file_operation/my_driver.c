#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

// function prototypes
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
};

static int etx_open(struct inode *inode, struct file *file){
    pr_info("drive open function called \r\n");
    return 0;
}
static int etx_release(struct inode *inode, struct file *file){
    pr_info("drive release function call \r\n");
    return 0;
}
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    pr_info("drive read function call \r\n");
    return 0;
}
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off){
    pr_info("drive write function call \r\n");
    return 0;
}
/*
* module function start
*/
static int __init start_func(void){
    pr_info("starting \r\n");
    pr_info("allocating Major number \r\n");
    if (alloc_chrdev_region(&dev, 0, 1, "k5_0_device") < 0){
        pr_err("can\'t allocate major number for device \r\n");
        return -1;
    }
    pr_info("major = %d minor = %d \n",MAJOR(dev), MINOR(dev));
    pr_info("kernel module inserted successfully \r\n");

    pr_info("creating cdev structure \r\n");
    cdev_init(&etx_cdev, &fops);

    pr_info("adding character device to the system \r\n");
    if(cdev_add(&etx_cdev,dev,1) < 0){
        pr_err("Can\'t create the character device \n");
        class_destroy(dev_class);
        return -1;
    }

    pr_info("creating struct class \r\n");
    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device"))){
        pr_err("Can\'t create the struct class \r\n");
        class_destroy(dev_class);
        return -1;
    }

    pr_info("creating device \r\n");
    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device"))){
        pr_err("Can\'t create the device \r\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    pr_info("device driver inserted \r\n");
    return 0;
}
/*
* module function end
*/
static void __exit end_func(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("end !...");
}

module_init(start_func);
module_exit(end_func);

MODULE_AUTHOR("nk");
MODULE_LICENSE("glp");
MODULE_DESCRIPTION("file operations");
MODULE_VERSION("0:0.5");

