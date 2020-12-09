#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define EXAM_MAJOR_NUM  888
#define EXAM_NAME      "EXAM"
#define EXAM_MAGIC_NUM  199
static void __iomem * ptr;

static int exam_open(struct inode *node, struct file *pfile)   // open
{
    ptr = ioremap(0x21E8000, 0x1000);
    printk ("Exam Serial Driver Opened!\n");
    return 0;
}

static ssize_t exam_read(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos)    // read
{
    printk ("exam_read enter\n");

    return 0;
}

static ssize_t exam_write(struct file *pfile, char __user *pBuff, size_t size, loff_t *filepos)    // write
{
    int i = 0;

    while(i<size)
    {
        if (*((volatile unsigned char *)(ptr + 0x21E80B4 - 0x21E8000)) & 0x00000010);
        
        else 
        {
            *((volatile unsigned char *)(ptr + 0x21E8040 - 0x21E8000)) = pBuff[i];
            i++;
        }

    }

    printk("exam_write enter\n");

     return -1;
}

static int exam_release(struct inode *node, struct file *pfile)    // close
{
    printk("Exam Serial Driver Released!\n");
    return 0;
}

static int exam_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
   if(_IOC_TYPE(cmd) != EXAM_MAGIC_NUM)
   {
      printk("Wrong Magic Number!\n");
      return -1;
   }
   
   switch(_IOC_NR(cmd))
   {
      case 10: // uart off
      printk("IOCTL SIZE -> %d\n", _IOC_SIZE(cmd));   // should be 4
      *((volatile unsigned int *)(ptr + 0x21E8080 - 0x21E8000)) = *((volatile unsigned int *)(ptr + 0x21E8080 - 0x21E8000))&(0xfffffffe);

      break;
      
      case 11: // uart on
      printk("IOCTL SIZE -> %d\n", _IOC_SIZE(cmd));   // should be 4
      *((volatile unsigned int *)(ptr + 0x21E8080 - 0x21E8000)) = *((volatile unsigned int *)(ptr + 0x21E8080 - 0x21E8000))|(0x00000001);
      break;

      default: break;
   }

   return 0;
}

static const struct file_operations exam_fops ={
    .owner = THIS_MODULE,
    .open = exam_open,
    .read = exam_read,
    .write = exam_write,
    .release = exam_release,
    .unlocked_ioctl = exam_ioctl,
};

int flagRegisterSuccess = 0;

int __init init_exam(void)
{
    int ret;
    printk("Exam Serial Driver Loaded!\n");
    ret = register_chrdev(EXAM_MAJOR_NUM, EXAM_NAME, &exam_fops);

    if( ret < 0 )   // fail
    {
        printk("Exam register_chrdev fail\n");
        return ret;
    }

    else
    flagRegisterSuccess = 1;
    
    return 0;
}

void __exit exit_exam(void)
{
    if( flagRegisterSuccess == 1)
    {
        flagRegisterSuccess = 0;
        unregister_chrdev (EXAM_MAJOR_NUM, EXAM_NAME);
    }

    printk("Exam Serial Driver Unloaded!\n");
}

module_init(init_exam);
module_exit(exit_exam);

MODULE_LICENSE("GPL");
