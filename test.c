#include <stdio.h>
#include <unistd.h>
#include <syscall.h>
#include <string.h>
#include "syscall.h"


int main()
{
	int a; 
	char ret[BUF_LEN] = {0};
	//a = syscall(_NR_kylinversion, ret, sizeof(ret));
	a = syscall(_NR_kylinversion, ret, BUF_LEN);
	printf("kylin kernel version:\n%s", ret);
	
	return 0;
}

