#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int AT, BT, FT, TAT, WT, id;
    int RBT;
    int completed : 1;
};

struct Node
{
    struct Process data;
    struct Node *next;
};

// struct queue{
//     struct Node* head;
// };

struct Node* RBTpriorityInsert(struct Node *queue, struct Process process)
{

    if (!queue)
    {
        queue = (struct Node *)malloc(sizeof(struct Node));
        queue->data = process;
        queue->next = NULL;
        return queue;
    }

    struct Node *t = queue;

    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = process;
    new->next = NULL;

    if (t->next == NULL)
    {
        if (t->data.RBT < new->data.RBT)
        {
            t->next = new;
        }
        else
        {
            new->next = t;
            queue = new;
        }
        return queue;
    }
    if (t->data.RBT > new->data.RBT)
        ;
    {
        new->next = t->next;
        queue = new;
        return queue;
    }

    while (t->next != NULL && t->next->data.RBT < new->data.RBT)
    {
        t = t->next;
    }

    new->next = t->next;
    t->next = new;
    return queue;
}

struct Process getLowestRBTProcess(struct Node *queue)
{
    struct Process *p =(struct Process *)malloc(sizeof(struct Process ));
    *p = queue->data;
    queue = queue->next;
    return *p;
}

void SortByAT(struct Process *process, int length)
{
    int swapped = 0;
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (process[j].AT > process[j + 1].AT)
            {
                // printf("\n\t %d and %d",process[j].AT,process[j + 1].AT);
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
        process[i].id = i;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    SortByAT(process, np);

    // for(int i=0;i<np;i++){
    //     printf("\n %d",process[i].AT);
    // }

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

    float totalTAT = 0, totalWT = 0;

    printf("Process | ArrivalTime | BurstTime | FinishTime | TurnAroundTime | WaitingTime");
    for (int i = 0; i < np; i++)
    {
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;

        totalTAT += process[i].TAT;
        totalWT += process[i].WT;
        printf("\n   P%d\t    %5d\t%5d\t      %5d\t     %5d\t    %5d", process[i].id + 1, process[i].AT, process[i].BT, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms", totalTAT / (float)np);
    printf("\nAverage Waiting Time: %4.3f ms", totalWT / (float)np);
}

void SJF()
{

    int np;
    printf("\n\t\tSJF Selected.\n No. of processes:");
    scanf("%d", &np);
    struct Process *process = (struct Process *)calloc(np, sizeof(struct Process));

    for (int i = 0; i < np; i++)
    {
        printf("For Process P%d:\n\tArrival time:", i + 1);
        scanf("%d", &process[i].AT);
        printf("\t Burst Time:");
        scanf("%d", &process[i].BT);
        process[i].id = i;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    // SortByAT(process,np);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 0;
    struct Node *processQueue = NULL;
    while (processCount != np)
    {
        // Insert Arriving processes in queue;
        for (int i = 0; i < np; i++)
        {
            if (process[i].AT == ctime)
            {
                processQueue=RBTpriorityInsert(processQueue, process[i]);
            }
        }

        if (cprocessRBT == 0)
        {
            // process completed, CPU free for new process
            if (cprocessIndx == -1)
            {
                // NO initial process
                cprocessIndx = getLowestRBTProcess(processQueue).id;
                cprocessRBT = process[cprocessIndx].BT;
            }
            else
            {
                // Fill data for completed process
                process[cprocessIndx].FT = ctime;
                process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
                process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

                // Add overhead for switching
                ctime += overheadTime;

                // Give CPU a new Process
                cprocessIndx = getLowestRBTProcess(processQueue).id;
                cprocessRBT = process[cprocessIndx].BT;
                processCount++;
            }
        }

        ctime++;
        cprocessRBT--;
    }


    // PRINT FINAL TABLE and AVERAGES
    float totalTAT = 0, totalWT = 0;

    printf("Process | ArrivalTime | BurstTime | FinishTime | TurnAroundTime | WaitingTime");
    for (int i = 0; i < np; i++)
    {
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;

        totalTAT += process[i].TAT;
        totalWT += process[i].WT;
        printf("\n   P%d\t    %5d\t%5d\t      %5d\t     %5d\t    %5d", process[i].id + 1, process[i].AT, process[i].BT, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms", totalTAT / (float)np);
    printf("\nAverage Waiting Time: %4.3f ms", totalWT / (float)np);
}

void main()
{

    int selection;
    printf("Select Schedular:\n1) FCFS\n2) SJF");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        FCFS();
        break;
    case 2:
        SJF();
        break;

    default:
        break;
    }
}