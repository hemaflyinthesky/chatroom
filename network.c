#include "network.h"

//分配内存、创建socket、准备地址、绑定、监听、连接
NetWork* init_nw(int type,short port,const char* ip,bool is_svr)
{
	NetWork* nw=malloc(sizeof(NetWork));
	nw->type=type;
	nw->is_svr=is_svr;
	nw->sock_fd=socket(AF_INET,type,0);
	if(0>nw->sock_fd)
	{
		perror("socket");
		free(nw);
		return NULL;
	}
	nw->addr.sin_family=AF_INET;
	nw->addr.sin_port=htons(port);
	nw->addr.sin_addr.s_addr=inet_addr(ip);
	nw->addrlen=sizeof(struct sockaddr_in);
	if(is_svr)
	{
		//绑定
		if(bind(nw->sock_fd,(SP)&nw->addr,nw->addrlen))
		{
			perror("bind");
			free(nw);
			return NULL;
		}
		if(type==SOCK_STREAM && listen(nw->sock_fd,20))
		{
			//监听 TCP
			perror("listen");
			free(nw);
			return NULL;
		}
	}
	else if(SOCK_STREAM==type)
	{
		//连接
		if(connect(nw->sock_fd,(SP)&nw->addr,nw->addrlen))
		{
			perror("connect");
			free(nw);
			return NULL;
		}
	}
	return nw;
}

//等待连接,只有type==SOCK_STREAM并且是服务器才能调用此函数
NetWork* accept_nw(NetWork* nw)
{
	if(SOCK_STREAM!=nw->type || !nw->is_svr)
	{
		printf("只有TCP且为服务器的NetWork对象才能调用accept_nw函数\n");
		return NULL;
	}
	// 为新的Network分配内容并初始化
	NetWork* cli_nw=malloc(sizeof(NetWork));
	cli_nw->type=nw->type;
	cli_nw->is_svr=true;
	cli_nw->addrlen=nw->addrlen;
	cli_nw->sock_fd=accept(nw->sock_fd,(SP)&cli_nw->addr,&cli_nw->addrlen);
	if(cli_nw->sock_fd<0)
	{
		perror("accpet");
		free(cli_nw);
		return NULL;
	}
	return cli_nw;
}

//具备send和sendto的发送函数
int send_nw(NetWork* nw,const void* buf,size_t len)
{
	if(SOCK_STREAM==nw->type) return send(nw->sock_fd,buf,len,0);
	else return sendto(nw->sock_fd,buf,len,0,(SP)&nw->addr,nw->addrlen);
}

//具备recv和recvfrom的接收函数
int recv_nw(NetWork* nw,void* buf,size_t len)
{
	if(SOCK_STREAM==nw->type) return recv(nw->sock_fd,buf,len,0);
	else return recvfrom(nw->sock_fd,buf,len,0,(SP)&nw->addr,&nw->addrlen);
}

//关闭socket并释放内存
void close_nw(NetWork* nw)
{
	close(nw->sock_fd);
	free(nw);
}

