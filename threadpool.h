#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "queue.h"

typedef void (*WorkFP)(void*);//线程真正业务逻辑函数格式

typedef struct Threadpool
{
	int thread_cnt;//线程数量
	pthread_t* tids;//线程id数组首地址
	Queue* store_buf;//队列仓库
	pthread_mutex_t flock;//队头互斥量
	pthread_mutex_t block;//队尾互斥量
	pthread_cond_t full;//满仓时条件变量
	pthread_cond_t empty;//空仓时条件变量
	WorkFP workfp;//线程业务逻辑函数
}Threadpool;

//创建线程池
Threadpool* create_threadpool(int thread_cnt,int store_cal,WorkFP workfp);

//启动线程池
void start_threadpool(Threadpool* threadpool);

//生产数据
void push_threadpool(Threadpool* threadpool,void* data);

//消费数据
void* pop_threadpool(Threadpool* threadpool);

//销毁线程池
void destory_threadpool(Threadpool* threadpool);

#endif //THREADPOOL_H
