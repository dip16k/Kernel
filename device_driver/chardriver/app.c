#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
main()
{
	char str[50];
	int fd;
	fd = open("/dev/dip",O_RDONLY);
	if(fd<0)
	{
		perror("open"); return;
	}
	read(fd,str,sizeof(str));

	printf(" kernel :%s\n",str);
}
