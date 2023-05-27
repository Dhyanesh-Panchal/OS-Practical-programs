#include <pthread.h>
#include <stdio.h>
#include <windows.h>
#include <unistd.h>
int n = 0;
int arr[100];
void addNumber()
{
    while (1)
    {
        printf("Enter the Number:");
        scanf("%d", &arr[n]);
        n++;
    }
}
void printSum()
{
    while (1)
    {
        sleep(2);
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

void main()
{
    pthread_t thread1;
    pthread_create(&thread1, NULL, addNumber, NULL);
    // myTurn();
    printSum();
    pthread_join(thread1, NULL); // This will wait for a thread to complete;
}