#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef struct NetWork
{
	int type;		//决定通信协议TCP、UDP类型
	int sock_fd;	//socket描述符
	struct sockaddr_in addr;//通信地址结构体
	socklen_t addrlen;
	bool is_svr;	//为真服务端
}NetWork;

typedef struct sockaddr* SP;

//分配内存、创建socket、准备地址、绑定、监听、连接
NetWork* init_nw(int type,short port,const char* ip,bool if_svr);

//等待连接,只有type==SOCK_STREAM并且是服务器才能调用此函数
NetWork* accept_nw(NetWork* nw);

//具备send和sendto的发送函数
int send_nw(NetWork* nw,const void* buf,size_t len);

//具备recv和recvfrom的接收函数
int recv_nw(NetWork* nw,void* buf,size_t len);

//关闭socket并释放内存
void close_nw(NetWork* nw);

#endif //NETWORK_H
