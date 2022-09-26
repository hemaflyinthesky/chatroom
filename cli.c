#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* run(void* arg)
{
    int tid=*(int*)arg;
    char buf[4096]={};
    size_t buf_size=sizeof(buf);
    for(;;)
    {
        int ret=recv(tid,buf,buf_size,0);
        if(ret<=0)
        {
            printf("over\n");
            close(tid);
            return EXIT_SUCCESS;
        }
        printf("recv:[%s]\n",buf);
    }
}
int main()
{
/*    //初始化网络库
    WSADATA wasDATA;
    if(WSAStartup(MAKEWORD(2,2),&wasDATA))
    {
        perror("WSAStartup");
        return -1;
    }*/
    int clifd=socket(AF_INET,SOCK_STREAM,0);
    if(0>clifd)
    {
        perror("socket");
        return EXIT_FAILURE;
    }
    struct sockaddr_in addr={};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5566);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int addrlen=sizeof(addr);
    char buf[4096]={};
    size_t bufsize=sizeof(buf);

	system("clear");
	printf("                             河马聊聊有知己聊天室\n");
	printf("-----------------------------------------------------------------------------\n");
	printf("\n\n请输入你的昵称:\n");
	scanf("%s",buf);
	printf("请等待...\n");
    if(connect(clifd,(struct sockaddr*)&addr,addrlen))
    {
        printf("connect");
        return EXIT_FAILURE;
    }
	system("clear");
	printf("欢迎进入聊天室！\n");
	printf("消息栏：\n\n");
    pthread_t tid;
    pthread_create(&tid,NULL,run,&clifd);
    send(clifd,buf,strlen(buf)+1,0);
    for(;;)
    {
        scanf("%s",buf);
        int ret=send(clifd,buf,strlen(buf)+1,0);
        if(0>=ret || strcmp(buf,"quit")==0)
        {
            printf("over\n");
            close(clifd);
            return EXIT_SUCCESS;
        }
    }
}
