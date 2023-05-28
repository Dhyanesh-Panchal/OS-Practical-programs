#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define semaphore int
#define bufferSize 5

semaphore mutex = 1;
semaphore full = 0;
semaphore empty = bufferSize;

/********** SEMAPHORE FUNCTIONS ***********/
void up(semaphore *lock)
{
    (*lock)++;
}

void down(semaphore *lock)
{
    while ((*lock) <= 0)
        ; // Loop untill the lock is released
    (*lock)--;
}
/****************************************/

/********** BUFFER FUNCTIONS ***********/
void insertBuffer(int item, int *buffer)
{
    buffer[full] = item;
}

int getItemFromBuffer(int *buffer)
{
    int item = buffer[0]; // Get data

    if (full == 0)
    {
        buffer[0]=0; // it represents empty
    }
    else
    {
        for (int i = 0; i < full; i++) // shift remaining Data
        {
            buffer[i] = buffer[i + 1];
        }
    }

    return item;
}

void printBuffer(int *buffer)
{
    printf("\nThe Buffer is: ");
    for (int i = 0; i <= full; i++)
    {
        (buffer[i]!=0)?(printf("%d, ", buffer[i])):(printf(""));
        // printf("%d, ", buffer[i]);
    }
}
/*************************************/

void *producer(void *buffer)
{
    int item;
    while (1)
    {
        item = rand(); // generate a random item.
        down(&empty);
        down(&mutex);

        insertBuffer(item, (int *)buffer);
        printf("\n\nPRODUCER ==> Inserted Item: %d  in buffer", item);
        printBuffer(buffer);

        up(&mutex);
        up(&full);
        sleep(1); // Just to make process visible
    }
}

void *consumer(void *buffer)
{
    int item;
    while (1)
    {
        down(&full);
        down(&mutex);

        item = getItemFromBuffer((int *)buffer);
        printf("\n\nCONSUMER ==> Removed Item %d from buffer", item);

        up(&mutex);
        up(&empty);

        printBuffer(buffer);
        printf("\nCONSUMER ==> Consuming Item %d", item);
        sleep(2); // Just to make process visible
    }
}

void main()
{
    int *buffer = (int *)calloc(bufferSize, sizeof(int));

    pthread_t producerThread, consumerThread;
    pthread_create(&producerThread, NULL, producer, buffer);
    pthread_create(&consumerThread, NULL, consumer, buffer);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    free(buffer);
}