#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define OFF 0
#define ON 1
#define GPIOCNT 8

long gpioLedInit(void);
void gpioLedSet(long);
void gpioLedFree(void);
long gpioKeyInit(void);
long gpioKeyGet(void);
void gpioKeyFree(void);
int gpioLed[] = {6,7,8,9,10,11,12,13};
int gpioKey[] = {16,17,18,19,20,21,22,23};

static int hello_init(void)
{
	long ret = 0;
	printk("led test module\n");
	long val = 0xff;
	ret = gpioLedInit();
	if(ret < 0)
		return ret;
	gpioLedSet(val);

	ret = gpioKeyInit();
	if(ret < 0)
		return ret;
	val = gpioKeyGet();
	printk("key : %#04x\n", (int)val);
//	gpioLedSet(val);
	
	return ret;
}

static void hello_exit(void)
{
	long val = 0x00;
	gpioLedSet(val);
	gpioLedFree();
	gpioKeyFree();
	printk("Goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);

long gpioLedInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioLed[i]);
		ret = gpio_request(gpioLed[i],gpioName);
		if(ret < 0) {
			printk("Failed request gpio%d error\n",gpioLed[i]);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_output(gpioLed[i],OFF);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n",gpioLed[i]);
			return ret;
		}
	}
	return ret;
}

void gpioLedSet(long val)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		gpio_set_value(gpioLed[i],val & (0x1 << i));
//		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
	}

}

void gpioLedFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioLed[i]);
	}
}
long gpioKeyInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioKey[i]);
		ret = gpio_request(gpioKey[i],gpioName);
		if(ret < 0) {
			printk("Failed request gpio%d error\n",6);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_input(gpioKey[i]);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n",6);
			return ret;
		}
	}
	return ret;
}
long gpioKeyGet(void)
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
void gpioKeyFree(void)
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
