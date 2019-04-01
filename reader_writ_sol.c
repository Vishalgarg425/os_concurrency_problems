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
#define BUFFER_SIZE 10
#define NO_WRIT_THREADS 13
#define NO_READ_THREADS 11

sem_t havetowait;
sem_t mutex;
atomic_int rc=0;
void *writer()
{   
    proberen(&havetowait);
    sleep(rand()%11);
    to_be_manipulated=rand()%1003;
    printf("variable has been manipulated to become %d\n",to_be_manipulated); 
    verhogen(&havetowait);  
    printf("reading operation completed\n\n");    
};
void *reader()
{
    proberen(&mutex);
    sleep(rand()%11);
    rc++;
    if(rc==0)
    proberen(&havetowait);
    verhogen(&mutex);
    printf("reading the variable %d\n",to_be_manipulated);
    printf("reading operation completed\n\n"); 
    proberen(&mutex);
    rc--;
    if(rc==0)
    verhogen(&havetowait);
    verhogen(&mutex);
       
};

int main(){
    srand(time(0));
    int size=BUFFER_SIZE;
    sem_init(&havetowait, 1);
    sem_init(&mutex,1);

   
    printf("start");

    pthread_t prod_t[(int)NO_WRIT_THREADS];
    for(int i=0;i<NO_WRIT_THREADS;i++)
    {
      pthread_create(&prod_t[i], NULL, writer, NULL);
    }
    pthread_t cons_t[NO_READ_THREADS];
    for(int i=0;i<NO_READ_THREADS;i++)
    {
      pthread_create(&cons_t[i], NULL, reader, NULL);
      
    }

    for(int i=0;i<NO_WRIT_THREADS;i++)
    {
      pthread_join(prod_t[i], NULL);
    }

    for(int i=0;i<NO_READ_THREADS;i++)
    {
      pthread_join(cons_t[i], NULL);
    } 
        printf("end");
}
