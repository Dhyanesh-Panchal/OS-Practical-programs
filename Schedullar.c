#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int AT, BT, FT, id;
};

void SortByAT(struct Process *process, int length)
{
    int swapped = 0;
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = i; j < length - i - 1; j++)
        {
            if (process[j].AT > process[j + 1].AT)
            {
                swapped = 1;
                struct Process temp;
                temp = process[j];
                process[j] = process[j + 1];
                process[j + 1] = temp;
            }
        }
        if (!swapped)
        {
            return;
        }
    }
}

void FCFS()
{

    int np;
    printf("\n\t\tFCFS Selected.\n No. of processes:");
    scanf("%d", &np);
    struct Process *process = (struct Process *)calloc(np, sizeof(struct Process));

    for (int i = 0; i < np; i++)
    {
        printf("For Process P%d:\n\tArrival time:", i + 1);
        scanf("%d", &process[i].AT);
        printf("\t Burst Time:");
        scanf("%d", &process[i].BT);
        process[i].id = i + 1;
    }

    SortByAT(process, np);

    for (int i = 0; i < np; i++)
    {
        printf("\nP%d\t%d\t%d", process[i].id, process[i].AT, process[i].BT);
    }
}

void main()
{

    int selection;
    printf("Enter the Choice:\n1) FCFS");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        FCFS();
        break;

    default:
        break;
    }
}