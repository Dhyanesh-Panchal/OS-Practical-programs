#include <stdio.h>
#include <pthread.h>

void *printHelloWorld()
{
    printf("\nHello World!");
}

void *printSumofNNumbers()
{
    printf("\nEnter the number of inputs:");
    int n;
    scanf("%d", &n);
    int sum = 0;
        printf("\nEnter number:");
    for (int i = 0; i < n; i++)
    {
        int num;
        scanf("%d", &num);
        sum += num;
    }

    printf("The sum is %d",sum);
}

void main()
{
    pthread_t thread1, thread2;
    pthread_create(&thread2, NULL, printSumofNNumbers, NULL);
    pthread_create(&thread1, NULL, printHelloWorld, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}