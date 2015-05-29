#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "vmm.h"


/* 访存请求 */
Ptr_MemoryAccessRequest ptr_memAccReq;
/* FIFO file descriptor */
int fifo;


int main()
{
	ptr_memAccReq = (Ptr_MemoryAccessRequest) malloc(sizeof(MemoryAccessRequest));
	do_request();
	if ((fifo=open("request",O_WRONLY)) < 0)
	{
		puts("req open fifo failed");
		return 1;
	}
	if (write(fifo, ptr_memAccReq, sizeof(MemoryAccessRequest)) < 0)
	{
		puts("req write failed");
		return 1;
	}
	return 0;
}


/* 产生访存请求 */
void do_request()
{
	char c;
	int i;
	char j;
	printf("no auto input 1\n");
	c = getchar();
	if (c=='1'){
                printf("process\n");
		scanf("%d",&i);
		ptr_memAccReq->processNum = i;
		printf("v adress\n");
		scanf("%d",&i);
		ptr_memAccReq->virAddr = i;
		printf("resquest\n");
		scanf("%d",&i);
		switch (i){
			case 0: //读请求
			{
				ptr_memAccReq->reqType = REQUEST_READ;
				printf("产生请求：\n地址：%lu\t类型：读取\n", ptr_memAccReq->virAddr);
				break;
			}
			case 1: //写请求
			{
				ptr_memAccReq->reqType = REQUEST_WRITE;
				printf("value\n");
				j = getchar();
				while (j=='\n') j = getchar();
				ptr_memAccReq->value = j;
				printf("产生请求：\n地址：%lu\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
				break;
			}
			case 2:
			{
				ptr_memAccReq->reqType = REQUEST_EXECUTE;
				printf("产生请求：\n地址：%lu\t类型：执行\n", ptr_memAccReq->virAddr);
				break;
			}
			default:
				break;
		}
	}
	else 
	{
		ptr_memAccReq->virAddr = random() % VIRTUAL_MEMORY_SIZE;
		/* 随机产生请求类型 */
		switch (random() % 3)		
		{				
			case 0: //读请求
			{
				ptr_memAccReq->reqType = REQUEST_READ;
				ptr_memAccReq->processNum = random() % 2;
				printf("产生请求：\n地址：%lu\t类型：读取\n", ptr_memAccReq->virAddr);
				break;
			}
			case 1: //写请求
			{
				ptr_memAccReq->reqType = REQUEST_WRITE;
				ptr_memAccReq->processNum = random() % 2;
				/* 随机产生待写入的值 */
				ptr_memAccReq->value = random() % 0xFFu;
				printf("产生请求：\n地址：%lu\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
				break;
			}
			case 2:
			{
				ptr_memAccReq->reqType = REQUEST_EXECUTE;
				ptr_memAccReq->processNum = random() % 2;
				printf("产生请求：\n地址：%lu\t类型：执行\n", ptr_memAccReq->virAddr);
				break;
			}
			default:
				break;
		}
	}
}
