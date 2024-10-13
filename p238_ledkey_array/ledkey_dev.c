#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/gpio.h>
#include <linux/moduleparam.h>

#define CALL_DEV_NAME "calldev"
#define CALL_DEV_MAJOR 230

#define OFF 0
#define ON 1
#define GPIOCNT 8
#define DEBUG

static long gpioLedInit(void);
static void gpioLedSet(long);
static void gpioLedFree(void);
static long gpioKeyInit(void);
static long gpioKeyGet(void);
static void gpioKeyFree(void);

static int gpioLed[] = {6,7,8,9,10,11,12,13};
static int gpioKey[] = {16,17,18,19,20,21,22,23};

static long gpioLedInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioLed[i]);
		ret = gpio_request(gpioLed[i],gpioName);
		if(ret < 0) {
#ifdef DEBUG
			printk("Failed request gpio%d error\n",gpioLed[i]);
#endif
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_output(gpioLed[i],OFF);
		if(ret < 0) {
#ifdef DEBUG
			printk("Failed direction_output gpio%d error\n",gpioLed[i]);
#endif
			return ret;
		}
	}
	return ret;
}

static void gpioLedSet(long val)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		gpio_set_value(gpioLed[i],val & (0x1 << i));
//		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
	}

}

static void gpioLedFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioLed[i]);
	}
}
static long gpioKeyInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioKey[i]);
		ret = gpio_request(gpioKey[i],gpioName);
		if(ret < 0) {
#ifdef DEBUG
			printk("Failed request gpio%d error\n",6);
#endif
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_input(gpioKey[i]);
		if(ret < 0) {
#ifdef DEBUG
			printk("Failed direction_output gpio%d error\n",6);
#endif
			return ret;
		}
	}
	return ret;
}
static long gpioKeyGet(void)
{
	long keyData=0;
	long temp;
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		temp = gpio_get_value(gpioKey[i]) << i;
		keyData |= temp;
	}
	return keyData;
}
static void gpioKeyFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioKey[i]);
	}
}

static int call_open(struct inode *inode, struct file *filp)
{
	int num = MINOR(inode->i_rdev);
	printk("call open-> minor : %d\n", num);
	num = MAJOR(inode->i_rdev);
	printk("call open-> major : %d\n", num);
	try_module_get(THIS_MODULE);
	return 0;
}
static loff_t call_llseek(struct file *filp, loff_t off, int whence)
{
	printk("call llseek -> off : %08X, whence : %08X\n", (unsigned int)off,whence);
	return 0x23;
}

static ssize_t call_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	char kbuf[128];
	int ret;
	int i;
	printk("call read -> buf : %08X, count : %08X\n", (unsigned int)buf, count);
	for(i=0;i<count;i++)
		kbuf[i] = i;

	kbuf[0] = (char)gpioKeyGet();
//	for(i=0;i<count;i++)
//		put_user(kbuf[i],buf++);
	ret=copy_to_user(buf,kbuf,count);	
	if(ret<0)
		return -EFAULT;

	return count;
}

static ssize_t call_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char kbuf[128];
	int ret;
//	int i;
	printk("call write -> buf : %08X, count : %08X\n", (unsigned int)buf, count);
//	for(i=0;i<count;i++)
//		get_user(kbuf[i],buf++);
	ret=copy_from_user(kbuf,buf,count); 
	if(ret<0)
		return -EFAULT;
	gpioLedSet(kbuf[0]);
	return count;
}

static long call_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("call ioctl -> cmd : %08X, arg : %08X\n",cmd, (unsigned int)arg);
	return 0x53;
}

static int call_release(struct inode *inode, struct file *filp)
{
	printk("call release \n");
	module_put(THIS_MODULE);
	return 0;
}
struct file_operations call_fops =
{
//	.owner = THIS_MODULE,
	.llseek = call_llseek,
	.read	= call_read,
	.write 	= call_write,
	.unlocked_ioctl	= call_ioctl,
	.open	= call_open,
	.release	= call_release,
};
static int call_init(void)
{	
	int result;
	printk("call call_init \n");
	result = register_chrdev(CALL_DEV_MAJOR,CALL_DEV_NAME, &call_fops);
	if(result < 0)	return result;
	
	result = gpioLedInit();
	if(result < 0)	return result;

	result = gpioKeyInit();
	if(result < 0)
		return result;
	return 0;
}
static void call_exit(void)
{	
	printk("call call_exit \n");
	unregister_chrdev(CALL_DEV_MAJOR,CALL_DEV_NAME);
	gpioLedFree();
	gpioKeyFree();
}

module_init(call_init);
module_exit(call_exit);
MODULE_AUTHOR("KCCI-AIOT");
MODULE_DESCRIPTION("test module");
MODULE_LICENSE("Dual BSD/GPL");


