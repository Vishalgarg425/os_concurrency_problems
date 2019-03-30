#include<stdio.h>
#include<unistd.h>
#include "semaphoremine.h" /* sem_t */
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