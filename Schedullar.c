#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int AT, BT, FT, TAT, WT, id;
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

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    SortByAT(process, np);

    // ctime = current time
    int ctime;
    ctime = process[0].AT;
    for (int i = 0; i < np; i++)
    {
        if (process[i].AT > ctime)
        {
            ctime = process[i].AT;
        }
        ctime += process[i].BT;
        process[i].FT = ctime;
        ctime += overheadTime;
    }

    int totalTAT=0,totalWT=0;

    printf("Process | ArrivalTime | BurstTime | FinishTime | TurnAroundTime | WaitingTime");
    for (int i = 0; i < np; i++)
    {
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;

        totalTAT+=process[i].TAT;
        totalWT+=process[i].WT;
        printf("\n   P%d\t    %5d\t%5d\t      %5d\t     %5d\t    %5d", process[i].id, process[i].AT, process[i].BT, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms",totalTAT/(float)np);
    printf("\nAverage Waiting Time: %4.3f ms",totalWT/(float)np);
}

void main()
{

    int selection;
    printf("Select Schedular:\n1) FCFS");
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