#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define semaphore int
#define bufferSize 5

semaphore mutex = 1;
semaphore full = 0;
semaphore empty = bufferSize;

void up(semaphore *lock)
{
    (*lock)++;
}
void down(semaphore *lock)
{
    while ((*lock) <= 0)
        ; // Loop untill the lock is released
    (*lock)--;
    // printf("Lock Acquired!");
}

void *producer(void *buffer)
{
    int item;
    while (1)
    {
        // item = rand(); // generate a random item.
        down(&mutex);
        // down(&empty);
        printf("\n\n\nPRODUCER ==> Lock Acquired!!!!");
        for (int i = 0; i < 5; i++)
        {
            printf("\n\tProduce,Produce,Produce......!");
            // sleep(1);
        }
        up(&mutex);
        printf("\nPRODUCER ==> Lock Release!!!!");
    }
}

void *consumer()
{
    int item;
    while (1)
    {
        down(&mutex);
        printf("\n\n\nCONSUMER ==> Lock Acquired!!!!");
        for (int i = 0; i < 3; i++)
        {
            printf("\n\tConsume,Consume,Consume......!");
            // sleep(1);
        }
        up(&mutex);
        printf("\nCONSUMER ==> Lock Release!!!!");
    }
}

void printBuffer(int *buffer)
{
    printf("");
}

void main()
{
    int *buffer = (int *)calloc(bufferSize, sizeof(int));

    pthread_t producerThread, consumerThread;
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
}