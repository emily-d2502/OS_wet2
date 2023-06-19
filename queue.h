#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue* Queue;
Queue create_queue();
void init(Queue q);
int is_empty(Queue q);
void enqueue(Queue q, int data);
int dequeue(Queue q);
void print(Queue q);
int size(Queue q);

#endif