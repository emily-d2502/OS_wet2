#include "segel.h"
#include "request.h"
#include "queue.h"
/*
    server.c: A very, very simple web server

    To run:
    ./server <portnum (above 2000)>

    Repeatedly handles HTTP requests sent to this port number.
    Most of the work is done within routines written in request.c
*/

#define EXPECTED_ARGC 3
void getargs(int argc, char *argv[], int *port, int *threads_num);


// sem_t sem;
pthread_cond_t c;
pthread_mutex_t m;
Queue req_waiting, req_handled;

void *thread_request_handler(void *ptr) {
    while (1) {
        pthread_mutex_lock(&m);
        while (is_empty(req_waiting)) {
            pthread_cond_wait(&c, &m);
        }
        int connfd = dequeue(req_waiting);
        enqueue(req_handled, connfd);
        pthread_mutex_unlock(&m);

        requestHandle(connfd);
        Close(connfd);
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    /*      parsing the command line      */
    int port, threads_num;
    struct sockaddr_in clientaddr;
    getargs(argc, argv, &port, &threads_num);


    /*      initialize a condition variable and queue      */
    // sem_init(&sem, 0, threads_num);
    pthread_cond_init(&c, NULL); pthread_mutex_init(&m, NULL);
    req_waiting = create_queue(); req_handled = create_queue();
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * threads_num);
    for (int i = 0; i < threads_num; ++i) {
        pthread_create(threads + i, NULL, thread_request_handler, NULL);
    }

    int listenfd = Open_listenfd(port);
    while (1) {
        int clientlen = sizeof(clientaddr);
        int connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
        pthread_mutex_lock(&m);
        enqueue(req_waiting, connfd);
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&m);
    }


}

void getargs(int argc, char *argv[], int *port, int *threads_num)
{
    if (argc < EXPECTED_ARGC) {
        fprintf(stderr, "Usage: %s <port> <threads>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *threads_num = atoi(argv[2]);
}
