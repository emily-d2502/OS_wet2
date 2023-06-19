#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#define ERORR -1

typedef struct Node {
    int data;
    struct Node *next;
} Node;

struct Queue {
    Node *front;
    Node *rear;
    int size;
};

Queue create_queue() {
    Queue q = (Queue) malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void init(Queue q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int is_empty(Queue q) {
    return q->front == NULL;
}

void enqueue(Queue q, int data) {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    ++q->size;

    if (is_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
        return;
    }

    q->rear->next = new_node;
    q->rear = new_node;
}

int dequeue(Queue q) {
    if (is_empty(q)) {
        return ERORR;
    }

    Node *temp = q->front;
    int data = q->front->data;

    if (q->front == q->rear) {
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front = q->front->next;
    }
    free(temp);
    --q->size;
    return data;
}

// Print the elements in the queue
void print(Queue q) {
    Node *temp = q->front;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int size(Queue q) {
    return q->size;
}



