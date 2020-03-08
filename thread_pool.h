#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>

typedef int t_bool;
#define t_ture 1
#define t_false 0

#define QMAX 200
#define P_ISDEAD 10
#define T_ISEM 11

typedef struct _thread_worker_t
{
    void *(*process)(void *args);
    void *data;
    struct _thread_worker_t * prev;
    struct _thread_worker_t * next;
}thread_worker_t;

typedef struct
{
    thread_worker_t * task_queue_header;
    pthread_t * thread_id;

    unsigned int num; 
    unsigned int queue_size;
    unsigned int queue_max;
    unsigned int tail;

    t_bool is_destory;
    t_bool is_full;

    pthread_mutex_t pool_mutex;
    pthread_cond_t pool_cond;


}thread_pool_t;

extern int thread_pool_init(thread_pool_t ** pool, int num);
extern int thread_pool_add_task(thread_pool_t ** pool, void * (*process)(void *), void * args);


#endif /* thread_pool.h */
