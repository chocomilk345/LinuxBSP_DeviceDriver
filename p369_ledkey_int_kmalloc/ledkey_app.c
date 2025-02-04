#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#pragma GCC diagnostic ignored "-Wunused-result"

#define DEVICE_FILENAME  "/dev/ledkey"

void print_OX(char data);
int main(int argc, char *argv[])
{
	char key_data,key_data_old=0;
	char val;
	int i;
	int dev;
	int ret;
	if(argc < 2)
	{
		printf("USAGE : %s ledVal[0x00~0xff]\n",argv[0]);
		return 1;
	}
    val = strtoul(argv[1],NULL,16);
    if(val<0 || 0xff<val)
    {
   		printf("Usage : %s ledValue(0x00~0xff)\n",argv[0]);
       	return 2;
    }
	dev = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );
	if( dev < 0 )
    {
        perror("open");
        return 1;
    }

	write(dev,&val,sizeof(val));
	do {
		usleep(100000);  //100MSec
		ret = read(dev, &key_data, sizeof(key_data));
		if(ret < 0)
		{
        	perror("read");
        	return 2;
		}
		if(key_data != key_data_old)
		{
			if(key_data)
			{
				val = 1 << key_data-1;
				print_OX(val);
				write(dev,&val,sizeof(key_data));
				if(key_data == 8)
					break;
			}
			key_data_old = key_data;
		}
	}while(1);
	close(dev);
   	return 0;
}
void print_OX(char data)
{
	int i;
	puts("0:1:2:3:4:5:6:7");
	for(i=0;i<8;i++)
	{
		if(data & (0x01 << i))
			putchar('O');
		else
			putchar('X');
		if(i != 7 )
			putchar(':');
		else
			putchar('\n');
	}
	putchar('\n');
}
