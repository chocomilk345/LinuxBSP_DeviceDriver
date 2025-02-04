#ifndef __IOCTL_H__
#define __IOCTL_H__

#define IOCTLTEST_MAGIC '6' 
typedef struct
{
	unsigned long timer_val;
} __attribute__((packed)) keyled_data;

#define TIMER_START 	_IO(IOCTLTEST_MAGIC, 0) 
#define TIMER_STOP 		_IO(IOCTLTEST_MAGIC, 1)
#define TIMER_VALUE		_IOW(IOCTLTEST_MAGIC, 2, keyled_data)
#define TIMER_MAXNR	    3
/*
#define IOCTLTEST_LEDON			_IO(IOCTLTEST_MAGIC, 3)
#define IOCTLTEST_GETSTATE		_IO(IOCTLTEST_MAGIC, 4)
#define IOCTLTEST_READ			_IOR(IOCTLTEST_MAGIC, 5,ioctl_test_info)
#define IOCTLTEST_WRITE			_IOW(IOCTLTEST_MAGIC, 6,ioctl_test_info)
#define IOCTLTEST_WRITE_READ	_IOWR(IOCTLTEST_MAGIC, 7,ioctl_test_info)
#define IOCTLTEST_KEYINIT 		_IO(IOCTLTEST_MAGIC, 8) 
#define IOCTLTEST_LEDINIT 		_IO(IOCTLTEST_MAGIC, 9) 
#define IOCTLTEST_LEDONOFF		_IO(IOCTLTEST_MAGIC, 10)
#define IOCTLTEST_MAXNR			11
*/
#endif
