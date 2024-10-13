#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/moduleparam.h>

#define OFF 0
#define ON 1
#define GPIOCNT 8
//#define DEBUG

static int ledval = 0;
static int gpioLed[] = {6,7,8,9,10,11,12,13};
static int gpioKey[] = {16,17,18,19,20,21,22,23};

module_param(ledval, int, 0);

static long gpioLedInit(void);
static void gpioLedSet(long);
static void gpioLedFree(void);
static long gpioKeyInit(void);
static long gpioKeyGet(void);
static void gpioKeyFree(void);

static int hello_init(void)
{
	long ret = 0;
	printk(KERN_INFO"led test module \n");
#ifdef DEBUG	
	printk("led test module [ ledval = %d ]\n", ledval);
#endif

	long val = 0xff;
	ret = gpioLedInit();
	if(ret < 0)
		return ret;
	//gpioLedSet(val);
	gpioLedSet(ledval);

	ret = gpioKeyInit();
	if(ret < 0)
		return ret;
	val = gpioKeyGet();
#ifdef DEBUG
	printk("key : %#04x\n", (int)val);
#endif
//	gpioLedSet(val);
	
	return ret;
}

static void hello_exit(void)
{
	printk("Goodbye world\n");
	gpioLedSet(0x00);
	gpioLedFree();
	gpioKeyFree();
}

module_init(hello_init);
module_exit(hello_exit);

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

MODULE_AUTHOR("KCC-AIOT");
MODULE_DESCRIPTION("test module");
MODULE_LICENSE("Dual BSD/GPL");
