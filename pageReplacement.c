#include <stdio.h>
#include <stdlib.h>

int n; // Size of input
int k;

void printFrame(int *frame)
{
    printf("\t");
    for (int i = 0; i < k; i++)
        printf("+----");
    printf("+\n\t");
    for (int i = 0; i < k; i++)
    {
        if (frame[i] == 0)
        {
            printf("|    ", frame[i]);
        }
        else
        {
            printf("| %2d ", frame[i]);
        }
    }
    printf("|\n\t");
    for (int i = 0; i < k; i++)
        printf("+----");
    printf("+");
}

int isHit(int x, int *frame)
{
    for (int i = 0; i < k; i++)
    {
        if (frame[i] == x)
            return 1;
    }
    return 0;
}

void FIFO(int *refStr, int *frame)
{
    int tMiss = 0, tHit = 0, prevIndx = 0;
    for (int i = 0; i < n; i++)
    {

        if (isHit(refStr[i], frame))
        {
            printf("\n\n\tPage:%d ==> Hit\n", refStr[i]);
            printFrame(frame);
            tHit++;
        }
        else
        {
            printf("\n\n\tPage:%d ==> Miss\n", refStr[i]);
            tMiss++;
            frame[prevIndx] = refStr[i];
            prevIndx = (prevIndx + 1) % k;
            printFrame(frame);
        }
    }

    printf("\nTotal Page fault: %d\nTotal Hits: %d\nHit Ratio: %0.2f \nMiss Ratio: %0.2f", tMiss, tHit, ((float)tHit) / n, ((float)tMiss) / n);
}

void LRU(int *refStr, int *frame)
{
}

void main()
{
    printf("Enter length of reference string:");
    scanf("%d", &n);
    int *refStr = (int *)calloc(n, sizeof(int));

    printf("Enter the space seperated reference string: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &refStr[i]);
    }

    printf("Enter the lenght of frame:");
    scanf("%d", &k);
    int *frame = (int *)calloc(k, sizeof(int));

    int selector;
    printf("Select the Algorithm:\n1) FIFO\n2) LRU\nEnter Choice:");
    scanf("%d", &selector);
    switch (selector)
    {
    case 1:
        FIFO(refStr, frame);
        break;
    case 2:
        LRU(refStr, frame);
        break;

    default:
        printf("invalid Choice");
        break;
    }
}