#include "./thread_pool.h"


void thread_routine(void * args)
{}

t_bool create_one_thread(pthread_t * threadid, pthread_attr_t * attr, void *(*ptr_start_routine)(void *), void * pool)
{
  pthread_create(threadid, attr, ptr_start_routine, pool);
  /*------------- add some verify
   *   */
}


int thread_pool_init(thread_pool_t ** pool, 
                    int num)
{

  /*----------------------- free pool*/
    *pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if((*pool) == NULL)//syserr
      return -1;
    pthread_mutex_init(&((*pool)->pool_mutex), NULL);
    pthread_cond_init(&((*pool)->pool_cond), NULL);
    (*pool)->is_destory = t_false;
    (*pool)->is_full = t_false;
    (*pool)->num = num;
    (*pool)->queue_max = QMAX ;
    (*pool)->tail = 0;
  /*----------------------- free task_queue */
    (*pool)->task_queue_header = (thread_worker_t *)calloc(QMAX, sizeof(thread_worker_t));
  /*----------------------- free thread_id in pool*/
    (*pool)->thread_id = (pthread_t *)calloc(1,num * sizeof(pthread_t));
    if((*pool)->thread_id == NULL)
    {
        free(*pool);
        *pool = NULL;
        return -1;
    }

    //create queue
    for(int index=0; index < num; index++)
    {
      t_bool ret = t_false;
      //default
      ret=create_one_pthread(&((*pool)->thread_id[index]),NULL,thread_routine,(void *)(*pool));
    }
  return 0;
  
}

int thread_pool_add_task(thread_pool_t ** pool,
            void * (*process)(void *),
                    void * args)
{

  pthread_mutex_lock(&((*pool)->pool_mutex));
  while(((*pool)->queue_size == (*pool)->queue_max) !((*pool)->is_destory))
  {
    // queue is full or pool is destory
    pthread_cond_wait(&((*pool)->pool_cond),&((*pool)->pool_mutex));
  }
  if ((*pool)->is_destory)
  {
    // pool is destory
    return P_ISDEAD;
  }

  if((*pool)->task_queue_header[(*pool)->tail].process != NULL )
    {
      //task is empty
      return T_ISEM;
    }

  pthread_mutex_unlock(&((*pool)->pool_mutex));

}
