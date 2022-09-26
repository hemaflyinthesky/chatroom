#include <stdio.h>
#include "threadpool.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int online[50];
Threadpool* threadpool;

typedef struct sockaddr* SP;

void sigint(int num)
{
	destory_threadpool(threadpool);
	printf("服务结束！\n");
	exit(0);
}

//真正执行的业务逻辑函数
void work(void* data) //消费者
{
	int clifd=*(int*)data,ret=0;
	char buf[4096]={};
	char cname[20];
	size_t buf_size=sizeof(buf);
	ret=recv(clifd,cname,sizeof(cname),0);
	sprintf(buf,"%s进入聊天室",cname);
	for(int i=0;i<50;i++)
	{
		if(online[i]!=0&&online[i]!=clifd)
		{
			ret=send(online[i],buf,strlen(buf)+1,0);
		}
	}
	for(;;)
	{
		// 接收请求
		ret=recv(clifd,buf,buf_size,0);
		if(strcmp(buf,"quit")==0||ret<=0)
		{
			sprintf(buf,"%s 退出聊天室",cname);
			for(int i=0;i<50;i++)
			{
				if(online[i]==clifd) 
				{
					online[i]=0;
					break;
				}
			}
			for(int i=0;i<50;i++)
			{
				if(online[i]!=0&&online[i]!=clifd)
				{
					ret=send(online[i],buf,strlen(buf)+1,0);
				}
			}
			close(clifd);
			free(data);
			return;
		}
//		printf("from %d recv:%s bits:%d\n",cli-d,buf,ret);
		// 响应请求
		char buf_send[4096]={};
		sprintf(buf_send,"%s:%s",cname,buf);
		for(int i=0;i<50;i++)
		{
			if(online[i]!=0&&online[i]!=clifd)
			{
	    		ret=send(online[i],buf_send,strlen(buf_send)+1,0);
				if(0>=ret)
				{
					printf("系统异常！\n");
					close(clifd);
					free(data);
					return;
				}
			}
		}
	}
}

int main(int argc,const char* argv[])
{
	signal(2,sigint);
	//创建socket
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(0>sockfd)
	{
		perror("socket");
		return EXIT_FAILURE;
	}
	//准备本机通信地址
	struct sockaddr_in addr={};
	addr.sin_family=AF_INET;
	addr.sin_port=htons(5566);
	//本机ip
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t addrlen=sizeof(addr);

	//绑定
	if(bind(sockfd,(SP)&addr,addrlen))
	{
		perror("bind");
		return EXIT_FAILURE;
	}
	//设置监听
	if(listen(sockfd,5))
	{
		perror("listen");
		return EXIT_FAILURE;
	}
	//创建线程池
	threadpool=create_threadpool(50,25,work);
	//启动线程池
	start_threadpool(threadpool);

	for(;;)
	{
		//生产者线程
		char buf[4096]={};
		int* cli_fdp=malloc(sizeof(int));
		*cli_fdp=accept(sockfd,(SP)&addr,&addrlen);
		if(*cli_fdp<0) continue;
		push_threadpool(threadpool,cli_fdp);
		for(int i=0;i<50;i++)
		{
			if(online[i]==0)
			{
				online[i]=*cli_fdp;
				break;
			}
		}
	}
}
