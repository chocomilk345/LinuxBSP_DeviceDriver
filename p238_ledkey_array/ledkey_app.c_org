#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <asm-generic/unistd.h>

#define DEVICE_FILENAME "/dev/calldev"

#pragma GCC diagnostic ignored "-Wunused-result"
int main(int argc, char *argv[])
{
	int val=0, key_data,key_data_old=0;
	int i;
	int dev;
	char buff;

	if(argc < 2)
	{
		printf("USAGE : %s ledVal[0x00~0xff]\n",argv[0]);
		return 1;
	}
//	printf("input value = ");
//	scanf("%x",&val);
	val = strtoul(argv[1],NULL,16);
    if(val<0 || 0xff<val)
    {
    	printf("Usage : %s ledValue(0x00~0xff)\n",argv[0]);
       	return 2;
    }
	dev = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
	if (dev < 0)
	{
		perror("open");
		return 0;
	}

	do {
		usleep(100000);  //100MSec
//		key_data = syscall(__NR_mysyscall,val);
		ssize_t ret = read(dev, &buff, sizeof(buff));
		if (ret < 0)
		{
			perror("read");
			close(dev);
			return 3;
		}

		key_data = (int)buff;
//		printf("key_data %08X\n", key_data);		
		if(key_data != key_data_old)
		{
			key_data_old = key_data;
			if(key_data)
			{
				ret = write(dev, &key_data, sizeof(key_data));
				if (ret < 0)
				{
					perror("write");
					close(dev);
					return 5;
				}

				val = key_data;
				puts("0:1:2:3:4:5:6:7");
				for(i=0;i<8;i++)
				{
					if(key_data & (0x01 << i))
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
			if(key_data == 0x80)
				break;
		}
	}while(1);

	close(dev);
	printf("mysyscall return value = %#04x\n",key_data);
    return 0;
}
