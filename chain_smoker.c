#include<stdio.h>
#include<unistd.h>
#include "semaphoremine.h" /* sem_t */

#define SMKR_NO 3//set whatever no u want keep it as large as possible
#define NO_SMKR_THREADS 113

sem_t mutex;
sem_t matab;
sem_t matbc;
sem_t matca; 

void *prepare(void*num)
{
    int* i = (int *)num;
    int smr=*i;
    if(smr==0)
    {
        proberen(&matbc);
        sleep(rand()%5);
        printf("smoker A is preparing");
        verhogen(&mutex);
    }
    else if(smr==1)
    {
        proberen(&matca);
        sleep(rand()%5);
        printf("smoker B is preparing");
        verhogen(&mutex); 
    }
    else if(smr==2)
    {
        proberen(&matab);
        sleep(rand()%5);
        printf("smoker C is preparing");
        verhogen(&mutex); 
    }
}

void *ChSmokrA()
{
      while(1) {
            proberen(&matbc);
            sleep(1);
            printf("smoker A is preparing\n");
            sleep(rand()%5);
            printf("Batch delivered\n");
            sleep(1);
            fflush(stdout);
            verhogen(&mutex);
      }
}

void *ChSmokrB()
{
      while(1) {
            proberen(&matca);
            sleep(1);
            printf("smoker B is preparing\n");
            sleep(rand()%5);
            printf("Batch delivered\n");
            sleep(1);
            fflush(stdout);
            verhogen(&mutex);
      }
}

void *ChSmokrC()
{
      while(1) {
            proberen(&matab);
            sleep(1);
            printf("smoker C is preparing\n");
            sleep(rand()%5);
            printf("Batch delivered\n");
            sleep(1);
            fflush(stdout);
            verhogen(&mutex);
      }
}

void *agent(void*num) 
{	
    while(1){
      proberen(&mutex);
      printf("\nnew delivery\n");
      int r = rand() % 3;
      if(r==0)verhogen(&matbc);
      else if(r==1)verhogen(&matca);
      else if(r==2)verhogen(&matab);
    }
}

int main(){
    srand(time(0));
    sem_init(&mutex, 1);
    sem_init(&matab, 0);
    sem_init(&matbc, 0);
    sem_init(&matca, 0);
   
    // printf("start\n");
    // int smkr_seq[NO_SMKR_THREADS];
    // int agnt_seq[NO_SMKR_THREADS];
    // for(int i=0;i<NO_SMKR_THREADS;i++){
    //     smkr_seq[i]=rand()%3;
    //     agnt_seq[i]=rand()%3;
    // }
    // pthread_t agnt_t[(int)NO_SMKR_THREADS];
    // pthread_t prod_t[(int)NO_SMKR_THREADS];
    // for(int i=0;i<NO_SMKR_THREADS;i++)
    // {
    //     // printf("%d",phil_seq[i]);
    //   pthread_create(&prod_t[i], NULL, prepare, &smkr_seq[i]);
    //   pthread_create(&agnt_t[i], NULL, agent, &agnt_seq[i]);
    // }

    // for(int i=0;i<NO_SMKR_THREADS;i++)
    // {
    //   pthread_join(prod_t[i], NULL);
    //   pthread_join(agnt_t[i], NULL);
    // }
    pthread_t thread_id[4];
    pthread_create(&thread_id[0], NULL, ChSmokrA, NULL);
    pthread_create(&thread_id[1], NULL, ChSmokrB, NULL);
    pthread_create(&thread_id[2], NULL, ChSmokrC, NULL);
    pthread_create(&thread_id[3], NULL, agent, NULL);

  pthread_join(thread_id[0], NULL);
  pthread_join(thread_id[1], NULL);
  pthread_join(thread_id[3], NULL);
  pthread_join(thread_id[2], NULL);
        printf("end");
}