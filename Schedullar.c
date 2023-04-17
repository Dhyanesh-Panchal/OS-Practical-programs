#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int AT, BT, FT, TAT, WT, id;
    int RBT; // Remaining Burst Time
    int arrived;
    int completed;
};

struct Node
{
    struct Process data;
    struct Node *next;
};

// void printProcess(struct Process p){
//     printf("\nProcess P%d\nAT=%d\nBT=%d\n")
// }

// struct queue{
//     struct Node* head;
// };

void printQueue(struct Node *queue)
{
    printf("\n\n\t\t");
    while (queue)
    {
        printf("\t%d", queue->data.id);
        queue = queue->next;
    }
}

struct Node *RBTpriorityInsert(struct Node *queue, struct Process process)
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
    {
        new->next = t;
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

struct Process getLowestRBTProcess(struct Node **queue)
{
    struct Process *p = (struct Process *)malloc(sizeof(struct Process));
    *p = (*queue)->data;
    struct Node *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
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

void SortByRBT(struct Process *process, int length)
{
    int swapped = 0;
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (process[j].RBT > process[j + 1].RBT)
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
        process[i].RBT = process[i].BT;
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
        process[i].RBT = process[i].BT;
        process[i].id = i;
        process[i].completed = 0;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    // SortByAT(process,np);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 1;
    struct Node *processQueue = NULL;
    while (processCount != np)
    {
        // Insert Arriving processes in queue;
        for (int i = 0; i < np; i++)
        {
            if (process[i].AT == ctime)
            {
                processQueue = RBTpriorityInsert(processQueue, process[i]);
                printQueue(processQueue);
            }
        }

        if (cprocessRBT == 0)
        {
            // process completed, CPU free for new process

            if (process[cprocessIndx].completed == 0)
            {
                // Fill data for completed process
                process[cprocessIndx].FT = ctime;
                process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
                process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;
                process[cprocessIndx].completed = 1;

                processCount++;
            }

            if (processQueue == NULL)
            {
                ctime++;
                continue;
            }

            if (cprocessIndx == -1)
            {
                // NO initial process
                cprocessIndx = getLowestRBTProcess(&processQueue).id;
                printf("\n%d", cprocessIndx);
                cprocessRBT = process[cprocessIndx].BT;
            }
            else
            {
                // // Fill data for completed process
                // process[cprocessIndx].FT = ctime;
                // process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
                // process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

                // processCount++;

                // Add overhead for switching
                ctime += overheadTime;

                // Give CPU a new Process
                cprocessIndx = getLowestRBTProcess(&processQueue).id;
                printf("\n%d  and processCount=%d", cprocessIndx, processCount);
                cprocessRBT = process[cprocessIndx].BT;
            }
        }

        ctime++;
        cprocessRBT--;
        process[cprocessIndx].RBT--;
    }

    // FOR LAST PROCESS
    process[cprocessIndx].FT = ctime - 1 + overheadTime + process[cprocessIndx].BT;
    process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
    process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

    // PRINT FINAL TABLE and AVERAGES
    float totalTAT = 0, totalWT = 0;

    printf("\n\nProcess | ArrivalTime | BurstTime | FinishTime | TurnAroundTime | WaitingTime");
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

void SRTN()
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
        process[i].RBT = process[i].BT;
        process[i].id = i;
        process[i].arrived = 0;
        process[i].completed = 0;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    SortByAT(process, np);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 0;

    struct Node *processQueue = NULL;

    printf("\n--------------------------");
    while (processCount != np)
    {
        // Insert Arriving processes in queue;
        for (int i = 0; i < np; i++)
        {
            if (process[i].AT <= ctime && process[i].arrived == 0)
            {
                // New Process arrives
                process[i].arrived = 1;

                if (cprocessIndx == -1)
                { // NO process initialized
                    cprocessIndx = i;
                    cprocessRBT = process[i].RBT;
                }
                else if (process[i].RBT < process[cprocessIndx].RBT)
                {
                    // Push running process to Queue
                    processQueue = RBTpriorityInsert(processQueue, process[cprocessIndx]);

                    if (process[i].AT != process[cprocessIndx].AT) // Process Switching due to PREMPTION
                    {
                        ctime += overheadTime; // Add overhead
                        printf("\n--------------------------");
                    }
                    cprocessIndx = i;
                    cprocessRBT = process[cprocessIndx].RBT;
                }
                else
                {
                    processQueue = RBTpriorityInsert(processQueue, process[i]);
                }
                // printQueue(processQueue);
            }
        }

        if (cprocessRBT == 0 && cprocessIndx != -1)
        {
            // process completed, CPU free for new process

            if (process[cprocessIndx].completed == 0)
            {
                // Fill data for completed process
                process[cprocessIndx].FT = ctime;
                process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
                process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;
                process[cprocessIndx].completed = 1;

                processCount++;
            }

            if (processQueue == NULL)
            {
                // NO PROCESS ZONE
                ctime++;
                printf("\nXXXXXXXXXXXXXXXXX");
                continue;
            }

            // if (cprocessIndx == -1)
            // {
            //     // NO initial process
            //     cprocessIndx = getLowestRBTProcess(&processQueue).id;
            //     printf("\n%d", cprocessIndx);
            //     cprocessRBT = process[cprocessIndx].RBT;
            // }
            else
            {
                // Add overhead for switching
                ctime += overheadTime;

                // Give CPU a new Process
                cprocessIndx = getLowestRBTProcess(&processQueue).id;
                // printf("\n%d  and processCount=%d", cprocessIndx, processCount);
                cprocessRBT = process[cprocessIndx].RBT;
                printf("\n--------------------------");
            }
        }

        ctime++;
        cprocessRBT--;
        process[cprocessIndx].RBT--;

        printf("\n %2d |  P%d  |   %2d  ", ctime, process[cprocessIndx].id, process[cprocessIndx].RBT);
    }

    process[cprocessIndx].FT = ctime - 1 + overheadTime + process[cprocessIndx].RBT;
    process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
    process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

    // PRINT FINAL TABLE and AVERAGES
    float totalTAT = 0, totalWT = 0;

    printf("\n\nProcess | ArrivalTime | BurstTime | FinishTime | TurnAroundTime | WaitingTime");
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

void PriorityNonPremptive()
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
        process[i].RBT = process[i].BT;
        process[i].id = i;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);
}

void main()
{

    int selection;
    printf("Select Schedular:\n1) FCFS\n2) SJF\n3) SRTN\n4)Priority (Non-Premptive)");
    scanf("%d", &selection);
    switch (selection)
    {
    case 1:
        FCFS();
        break;
    case 2:
        SJF();
        break;
    case 3:
        SRTN();
        break;
    case 4:
        PriorityNonPremptive();
        break;

    default:
        break;
    }
}