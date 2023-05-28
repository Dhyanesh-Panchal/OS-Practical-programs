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
    //****No NEED==>// while((*lock)>=bufferSize); // This condition is for Full (when buffer is full, it will stop producer to produce)

    (*lock)++;
}

void down(semaphore *lock)
{
    while ((*lock) <= 0)
        ; // Loop untill the lock is released
    (*lock)--;
    // printf("Lock Acquired!");
}
/****************************************/

/********** BUFFER FUNCTIONS ***********/
void insertBuffer(int item, int *buffer)
{
    buffer[full] = item;
}

int getItemFromBuffer(int *buffer)
{
    int item = buffer[0];          // Get data
    for (int i = 0; i < full; i++) // shift remaining Data
    {
        buffer[i] = buffer[i + 1];
    }

    return item;
}

void printBuffer(int *buffer)
{
    printf("\n\n The Buffer is: ");
    for (int i = 0; i <= full; i++)
    {
        printf("%d, ", buffer[i]);
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
        // printf("\n\n\nPRODUCER ==> Lock Acquired!!!!");
        insertBuffer(item, (int *)buffer);
        printf("\nPRODUCER ==> Inserted Item: %d  in buffer", item);
        printBuffer(buffer);
        sleep(1); // Just to make process visible
        // printf("\nPRODUCER ==> Lock Release!!!!");
        up(&mutex);
        up(&full);
    }
}

void *consumer(void *buffer)
{
    int item;
    while (1)
    {
        down(&full);
        down(&mutex);
        // printf("\n\n\nCONSUMER ==> Lock Acquired!!!!");
        item = getItemFromBuffer((int *)buffer);
        // printf("\nCONSUMER ==> Lock Release!!!!");
        sleep(1); // Just to make process visible
        up(&mutex);
        up(&empty);

        printf("\nCONSUMER ==> Consuming Item %d", item);
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