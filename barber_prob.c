#include<stdio.h>
#include<unistd.h>
// #include "semaphoremine.h" /* sem_t */
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
#define NO_OF_SEATS 13//set whatever no u want keep it as large as possible
#define NO_CSTR_THREADS 4

sem_t barbmutex;
sem_t seatmutex;
sem_t cstmrmutex;
int no_of_free_seats=13;
// 
void* barber(void* num) 
{ 
    while(1){
        proberen(&cstmrmutex);
  		proberen(&seatmutex);
        sleep(rand()%3);
        no_of_free_seats+=1; 
        printf("done cutting\n"); 
        printf("%d seats left\n\n",no_of_free_seats); 
        fflush(stdout);
        verhogen(&barbmutex);
        verhogen(&seatmutex);  
    } 
}
void* customer(void* num) 
{ 
    while(1){
  		proberen(&seatmutex);
          if(no_of_free_seats>0){
              no_of_free_seats--;
              printf("new customer arrived\n");
              printf("%d seats left\n\n",no_of_free_seats);
              fflush(stdout);
              verhogen(&cstmrmutex);
              verhogen(&seatmutex);
              proberen(&barbmutex);
          }
          else{
              verhogen(&seatmutex);
          }
    } 
}
int main(){
    srand(time(0));
    sem_init(&barbmutex, 0);
    sem_init(&seatmutex, 1);
    sem_init(&cstmrmutex, 0);
    printf("start\n");
    pthread_t barb_t;
    pthread_create(&barb_t, NULL, barber, NULL);
    pthread_t cstr_t[(int)NO_CSTR_THREADS];
   for(int i=0;i<NO_CSTR_THREADS;i++)
    {
      pthread_create(&cstr_t[i], NULL, customer, NULL);
    }
    for(int i=0;i<NO_CSTR_THREADS;i++)
    {
      pthread_join(cstr_t[i], NULL);
    }
    pthread_join(barb_t, NULL);
        printf("end");
}
