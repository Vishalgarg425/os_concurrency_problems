#include<stdio.h>
#include<unistd.h>
#include "semaphoremine.h" /* sem_t */

#define PHIL_NO 5//set whatever no u want keep it as large as possible
#define NO_PHIL_THREADS 113

sem_t S[(int)PHIL_NO];
sem_t mutex;
int last_pointer;

// 
void* philospher(void* num) 
{ 
        sleep(rand()%5);
        int* i = (int *)num; 
  		proberen(&mutex);
        fflush(stdout);
        proberen(&S[*i]);
  		proberen(&S[(*i + 1) % 5]);
        verhogen(&mutex);  
        sleep(rand()%3);
        int nhola=*i;
        printf("philospher %d is eating\n\n",nhola+1);
        // fflush(stdout);
        // 
  		verhogen(&S[*i]);
  		verhogen(&S[(*i + 1) % 5]);
        fflush(stdout);
        // verhogen(&mutex);  
}
int main(){
    srand(time(0));
   sem_init(&S[0], 1);
   sem_init(&S[1], 1);
    sem_init(&S[2], 1);
    sem_init(&S[3], 1);
    sem_init(&S[4], 1);
    sem_init(&mutex, 1);
   
    printf("start\n");
    int phil_seq[NO_PHIL_THREADS];
    for(int i=0;i<NO_PHIL_THREADS;i++){
        phil_seq[i]=rand()%5;
    }
    pthread_t prod_t[(int)NO_PHIL_THREADS];
   for(int i=0;i<NO_PHIL_THREADS;i++)
    {
        // printf("%d",phil_seq[i]);
      pthread_create(&prod_t[i], NULL, philospher, &phil_seq[i]);
    }
    // pthread_t cons_t[NO_CONS_THREADS];
    // for(int i=0;i<NO_CONS_THREADS;i++)
    // {
    //   pthread_create(&cons_t[i], NULL, consumer, NULL);
      
    // }

    for(int i=0;i<NO_PHIL_THREADS;i++)
    {
      pthread_join(prod_t[i], NULL);
    }

    // for(int i=0;i<NO_CONS_THREADS;i++)
    // {
    //   pthread_join(cons_t[i], NULL);
    // } 
        printf("end");
}