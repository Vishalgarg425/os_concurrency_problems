#include <stdio.h>
// #include <threads.h>
#include<pthread.h>
#include<stdlib.h> 
#include <stdatomic.h>

atomic_int atomic;
typedef struct 
{
        atomic_int semaph;
        pthread_mutex_t wait;
} sem_t;

void  sem_init(sem_t* sem, unsigned int value){
        sem->semaph=value;
}

void proberen(sem_t *sem){
    pthread_mutex_lock(&sem->wait); 
    while (1)  {
          if (sem->semaph <= 0) 
          {
             pthread_mutex_unlock(&sem->wait);
             continue;
          } 
          else 
          {
              pthread_mutex_unlock(&sem->wait);
              sem->semaph = sem->semaph - 1;
             ;break;
          }
       }


       

       
}
/* verhogen - to increment */
void verhogen(sem_t *sem)
{ 
    // pthread_mutex_lock(&sem->lock1);  
    sem->semaph = sem->semaph + 1;
    // pthread_mutex_unlock(&sem->lock1);
}
