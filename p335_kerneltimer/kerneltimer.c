#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/moduleparam.h>

#define DEBUG 1

static int timerVal = 100;
module_param(timerVal,int,0);
static int ledVal = 0;
module_param(ledVal,int,0);
struct timer_list timerLed;

#define GPIOLEDCNT 8
#define GPIOKEYCNT 8
#define OFF 0
#define ON 1
#define GPIOCNT 8

static int ledval = 0;
static int gpioLed[] = {6,7,8,9,10,11,12,13};
static int gpioKey[] = {16,17,18,19,20,21,22,23};

module_param(ledval, int, 0);

static int gpioLedInit(void);
static void gpioLedSet(long);
static void gpioLedFree(void);
static int gpioKeyInit(void);
static int gpioKeyGet(void);
static void gpioKeyFree(void);

static int  gpioLedInit(void)
{
    int i;
    int ret = 0;
    char gpioName[10];
    for(i=0;i<GPIOLEDCNT;i++)
    {
        sprintf(gpioName,"led%d",i);
        ret = gpio_request(gpioLed[i], gpioName);
        if(ret < 0) {
            printk("Failed Request gpio%d error\n", 6);
            return ret;
        }
    }
    for(i=0;i<GPIOLEDCNT;i++)
    {
        ret = gpio_direction_output(gpioLed[i], OFF);
        if(ret < 0) {
            printk("Failed direction_output gpio%d error\n", 6);
         return ret;
        }
    }
    return ret;
}
static void gpioLedSet(long val)
{
    int i;
    for(i=0;i<GPIOLEDCNT;i++)
    {
        gpio_set_value(gpioLed[i], (val>>i) & 0x01);
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

static int gpioKeyInit(void)
{
    int i;
    int ret=0;
    char gpioName[10];
    for(i=0;i<GPIOKEYCNT;i++)
    {
        sprintf(gpioName,"key%d",i);
        ret = gpio_request(gpioKey[i], gpioName);
        if(ret < 0) {
            printk("Failed Request gpio%d error\n", 6);
            return ret;
        }
    }
    for(i=0;i<GPIOKEYCNT;i++)
    {
        ret = gpio_direction_input(gpioKey[i]);
        if(ret < 0) {
            printk("Failed direction_output gpio%d error\n", 6);
         return ret;
        }
    }
    return ret;
}

static int  gpioKeyGet(void)
{
    int i;
    int ret;
    int keyData=0;
    for(i=0;i<GPIOKEYCNT;i++)
    {
        ret=gpio_get_value(gpioKey[i]) << i;
        keyData |= ret;
    }
    return keyData;
}
static void gpioKeyFree(void)
{
    int i;
    for(i=0;i<GPIOKEYCNT;i++)
    {
        gpio_free(gpioKey[i]);
    }
}

void kerneltimer_func(struct timer_list *t);
void kerneltimer_registertimer(unsigned long timeover)
{
	timerLed.expires = get_jiffies_64() + timeover;
	timer_setup( &timerLed,kerneltimer_func,0);
	add_timer( &timerLed);
}
void kerneltimer_func(struct timer_list *t)
{
#if DEBUG
	printk("ledVal : %#04x\n",(unsigned int)(ledVal));
#endif
	gpioLedSet(ledVal);

	ledVal = ~ledVal & 0xff;
	mod_timer(t,get_jiffies_64() + timerVal);
}

int kerneltimer_init(void)
{
#if DEBUG
	printk("timerVal : %d, sec : %d \n",timerVal,timerVal/HZ);
#endif
	kerneltimer_registertimer(timerVal);

	long ret = 0;
    ret = gpioLedInit();
    if(ret < 0) return ret;
    ret = gpioKeyInit();
    if(ret < 0) return ret;

	return 0;
}
void kerneltimer_exit(void)
{
	if(timer_pending(&timerLed))
		del_timer(&timerLed);
	printk("kerneltimer_exit\n");
	gpioLedSet(0x00);
	gpioLedFree();
	gpioKeyFree();
}

module_init(kerneltimer_init);
module_exit(kerneltimer_exit);

MODULE_AUTHOR("KCCI_AIOT KSH");
MODULE_DESCRIPTION("led key test module");
MODULE_LICENSE("Dual BSD/GPL");

	
