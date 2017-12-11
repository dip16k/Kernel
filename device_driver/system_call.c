#include<stdio.h>
#include<unistd.h>
main()
{
	long int z;
		z=syscall(353,4,5);
		perror("syscall");
	printf("system call return =%ld\n",z);
}
