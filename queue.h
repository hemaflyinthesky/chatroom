#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct Queue
{
	void** arr;//内存首地址
	size_t cal;//容量
	int front;//队头下标
	int back;//队尾下标
}Queue;

//创建队列
Queue* create_queue(size_t cal);

//销毁队列
void destory_queue(Queue* queue);

//队空
bool empty_queue(Queue* queue);

//队满
bool full_queue(Queue* queue);

//入队
void push_queue(Queue* queue,void* arg);

//出队
void pop_queue(Queue* queue);

//队头
void* front_queue(Queue* queue);

//队尾
void* back_queue(Queue* queue);

#endif //QUEUE_H
