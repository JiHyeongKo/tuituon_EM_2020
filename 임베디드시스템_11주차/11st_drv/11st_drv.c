#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define HELLO_MAJOR_NUM 4095
#define HELLO_NAME      "SO_WHAT"

// static function is only used in 11st_drv.c
static int hello_open(struct inode *node, struct file *pfile)   // open
{
    printk("hello_open enter\n");
    return 0;
}

static ssize_t hello_read(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos)    // read
{
    char arrData[4] = {'1', '2', '3', '4'};
    printk("hello_read enter\n");
    
    if( size >= sizeof(arrData) )
    {
        copy_to_user(pBuff, arrData, sizeof(arrData));  //  to, from, size
        return sizeof(arrData);
    }

    return -1;
}

static ssize_t hello_write(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos)    // write
{
    char arrData[4];
    printk("hello_write enter\n");

     if( size >= sizeof(arrData) )
     {
        copy_from_user(arrData, pBuff, sizeof(arrData));   // to, from, size
        return sizeof(arrData);
     }

     return -1;
}

static int hello_release(struct inode *node, struct file *pfile)    // close
{
    printk("hello_release enter\n");
    return 0;
}

static const struct file_operations hello_fops ={
    .owner = THIS_MODULE,
    .open = hello_open,
    .read = hello_read,
    .write = hello_write,
    .release = hello_release,
};

int flagRegisterSuccess = 0;

int __init init_hello(void)
{
    int ret;
    printk("Hello World\n");
    ret = register_chrdev(HELLO_MAJOR_NUM, HELLO_NAME, &hello_fops);
    
    if( ret < 0 )   // fail
    {
        printk("Hello register_chrdev fail\n");
        return ret;
    }

    else
    flagRegisterSuccess = 1;
    
    return 0;
}

void __exit exit_hello(void)
{
    if( flagRegisterSuccess == 1)
    {
        flagRegisterSuccess = 0;
        unregister_chrdev (HELLO_MAJOR_NUM, HELLO_NAME);
    }

    printk("See Ya~\n");
}

module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");