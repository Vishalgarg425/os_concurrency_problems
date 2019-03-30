#include<stdio.h>
#include<unistd.h>
#include "semaphoremine.h" /* sem_t */
#define BUFFER_SIZE 10
#define NO_PROD_THREADS 13
#define NO_CONS_THREADS 8
typedef struct 
{
     int val;
} item;

sem_t fill;
sem_t empty;
sem_t mutex;

item buffer[(int)BUFFER_SIZE];
int last_pointer;

void *producer()
{   
    sleep(rand()%5);
    int value=rand()%113;
    item np;
    np.val=value;
    proberen(&fill);
    proberen(&mutex);
    if(last_pointer<BUFFER_SIZE)
   {
       buffer[last_pointer]=np;
       last_pointer++; 
       printf("produced %d\n",np.val);
   }
   else
   {
       printf("this section code should not be reached");
   }
    verhogen(&mutex);
    verhogen(&empty);  


    printf("producer has completed its execution\n\n");    
};
void *consumer()
{
    proberen(&empty);
    proberen(&mutex);
    sleep(rand()%11);
    if(last_pointer>0)
    {
        printf("consuming item %d\n",buffer[--last_pointer].val);
    }
    verhogen(&mutex);
    verhogen(&fill);
    printf("consumer has completed its execution\n\n");    

};

int main(){
    srand(time(0));
   last_pointer=0;
   int size=BUFFER_SIZE;
   sem_init(&empty, 0);
   sem_init(&fill, size);
   sem_init(&mutex,1);

   
    printf("start\n");

    pthread_t prod_t[(int)NO_PROD_THREADS];
   for(int i=0;i<NO_PROD_THREADS;i++)
    {
      pthread_create(&prod_t[i], NULL, producer, NULL);
    }
    pthread_t cons_t[NO_CONS_THREADS];
    for(int i=0;i<NO_CONS_THREADS;i++)
    {
      pthread_create(&cons_t[i], NULL, consumer, NULL);
      
    }

    for(int i=0;i<NO_PROD_THREADS;i++)
    {
      pthread_join(prod_t[i], NULL);
    }

    for(int i=0;i<NO_CONS_THREADS;i++)
    {
      pthread_join(cons_t[i], NULL);
    } 
        printf("end");
}