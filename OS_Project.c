#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#define size 5

char shared_space[size];
char c='A';
void *producer(void *);
void *consumer(void *);

sem_t prod;
sem_t cons;

int main()
{
    long int s=syscall(335);
    printf("System Call : sys_mulitple producers mulitple consumers : returned %ld \n",s);
    pthread_t prod_thread[size];
    pthread_t cons_thread[size];

    sem_init(&prod, 0, size);
    sem_init(&cons, 0, 0);
    int i;

    for ( i = 0; i < size ; i++)
    {
        pthread_create(&prod_thread[i], NULL, producer , (void *)&i);
        pthread_join(prod_thread[i],NULL);
    }

    for (i = 0; i < size ; i++)
    {
        pthread_create(&cons_thread[i], NULL,consumer , (void *)&i);
        pthread_join(cons_thread[i],NULL);
    }


    sem_destroy(&prod);
    sem_destroy(&cons);

    return 0;
}

void *producer(void *prod_no)
{
    int x = *((int*)prod_no);
    sem_wait(&prod);
    c++;
    shared_space[x]=c;
    printf("Producer No %d Start Producing \n" , x );
    printf("Producer No %d Producing %c \n",x,c);
    sem_post(&cons);
}

void *consumer(void *consumer_no)
{
    int x = *((int*)consumer_no);
    sem_wait(&cons);
    printf("Consumer No %d Start Consuming \n" ,x );
    printf("Consumer No %d Consuming %c \n",x,shared_space[x]);
    sem_post(&prod);
}