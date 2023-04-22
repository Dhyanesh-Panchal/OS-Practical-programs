#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int AT, BT, FT, TAT, WT, id, priority;
    int RBT; // Remaining Burst Time
    int arrived;
    int completed;
};

struct Node
{
    struct Process data;
    struct Node *next;
};

struct ChartNode
{
    int processIndx;
    int currentTime;
    struct ChartNode *next;
};

struct ChartNode *chartInsert(struct ChartNode *chart, int processIndx, int currentTime)
{
    struct ChartNode *new = (struct ChartNode *)malloc(sizeof(struct ChartNode));
    new->processIndx = processIndx;
    new->currentTime = currentTime;
    new->next = NULL;

    if (chart == NULL)
    {
        return new;
    }

    struct ChartNode *t = chart;
    while (t->next != NULL)
    {
        t = t->next;
    }

    t->next = new;
    return chart;
}

// get length for the Ganttchart
int getLength(struct ChartNode *chart)
{
    int length = 0;
    struct ChartNode *t = chart;
    while (t != NULL)
    {
        length++;
        if (t->processIndx == -2)
        {
            while (t && t->processIndx == -2)
            {
                t = t->next;
            }
        }
        else
        {
            t = t->next;
        }
    }

    return length;
}
void printChart(struct ChartNode *chart)
{
    printf("\nGantt Chart :\n\n");
    int length = getLength(chart);

    for (int i = 0; i < length; i++)
    {
        printf("+-------");
    }
    printf("+\n");
    struct ChartNode *t = chart;
    while (t != NULL)
    {
        if (t->processIndx == -1)
        {
            printf("|--OH-- ");
        }
        else if (t->processIndx == -2)
        {
            printf("|--NP-- ");
            while (t->next != NULL && t->next->processIndx == -2) // Noprocess Zone
                t = t->next;
        }
        else
        {
            printf("|P%d\t", t->processIndx);
        }
        t = t->next;
    }
    printf("|\n");
    for (int i = 0; i < length; i++)
    {
        printf("+-------");
    }
    printf("+\n");
    t = chart;
    while (t != NULL)
    {
        if (t->processIndx == -2)
        {
            printf("%2d\t", t->currentTime);
            while (t->next != NULL && t->next->processIndx == -2) // Noprocess Zone
                t = t->next;
        }
        else
        {
            printf("%2d\t", t->currentTime);
        }
        t = t->next;
    }
}

void printQueue(struct Node *queue)
{
    printf("\n\n\t\t");
    while (queue)
    {
        printf("\t%d", queue->data.id);
        queue = queue->next;
    }
}

// processQueue Insertion based on RBT, For SRTN and SJF.
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

// processQueue insertion based on priority for priority based Scheduling.
struct Node *priorityInsert(struct Node *queue, struct Process process)
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
        if (t->data.priority < new->data.priority)
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
    if (t->data.priority > new->data.priority)
    {
        new->next = t;
        queue = new;
        return queue;
    }

    while (t->next != NULL && t->next->data.priority < new->data.priority)
    {
        t = t->next;
    }

    new->next = t->next;
    t->next = new;
    return queue;
}

struct Process getLowestProcess(struct Node **queue)
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
    printf("\n\t\tFisrt Come First Serve Selected.\n No. of processes:");
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
    printf("\n\t\tShortest Job First Selected.\n No. of processes:");
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
        process[i].arrived = 0;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 1;
    struct Node *processQueue = NULL;
    while (processCount != np)
    {
        // Insert Arriving processes in queue;
        for (int i = 0; i < np; i++)
        {
            if (process[i].AT <= ctime && process[i].arrived == 0)
            {
                processQueue = RBTpriorityInsert(processQueue, process[i]);
                // printQueue(processQueue);
                process[i].arrived = 1;
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
                cprocessIndx = getLowestProcess(&processQueue).id;
                printf("\n%d", cprocessIndx);
                cprocessRBT = process[cprocessIndx].BT;
            }
            else
            {

                ctime += overheadTime;

                // Give CPU a new Process
                cprocessIndx = getLowestProcess(&processQueue).id;
                printf("\n%d  and processCount=%d", cprocessIndx, processCount);
                cprocessRBT = process[cprocessIndx].BT;
            }
        }

        ctime++;
        cprocessRBT--;
        process[cprocessIndx].RBT--;
    }

    // FOR LAST PROCESS
    process[cprocessIndx].FT = ctime - 1 + process[cprocessIndx].BT;
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
    printf("\n\t\tShortest Remaining Time Next (SRTN) Selected.\n No. of processes:");
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

    // SortByAT(process, np);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 0;

    struct Node *processQueue = NULL;
    struct ChartNode *gantt_chart = NULL;

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

                    gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime); // new process start in ganttchart
                }
                else if (process[i].RBT < process[cprocessIndx].RBT)
                {
                    // Push running process to Queue
                    processQueue = RBTpriorityInsert(processQueue, process[cprocessIndx]);

                    if (overheadTime != 0) // Process Switching due to PREMPTION
                    {
                        gantt_chart = chartInsert(gantt_chart, -1, ctime); // pushing overhead time to ganttchart
                        ctime += overheadTime;                             // Add overhead
                    }
                    cprocessIndx = i;
                    cprocessRBT = process[cprocessIndx].RBT;

                    gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime);
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
                if (processCount == np)
                {
                    break;
                }
            }

            if (processQueue == NULL) // for last process it will not perform
            {
                gantt_chart = chartInsert(gantt_chart, -2, ctime); // Add noProccess Zone to Ganttchart
                // NO PROCESS ZONE
                ctime++;
                cprocessIndx = -1;
                continue;
            }

            else
            {
                if (overheadTime != 0)
                {
                    gantt_chart = chartInsert(gantt_chart, -1, ctime); // pushing overhead time to ganttchart
                    // Add overhead for switching
                    ctime += overheadTime;
                }

                // Give CPU a new Process
                cprocessIndx = getLowestProcess(&processQueue).id;
                // printf("\n%d  and processCount=%d", cprocessIndx, processCount);
                cprocessRBT = process[cprocessIndx].RBT;

                gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime);
            }
        }

        ctime++;
        cprocessRBT--;
        process[cprocessIndx].RBT--;
    }

    // process[cprocessIndx].FT = ctime - 1 + overheadTime + process[cprocessIndx].RBT;
    // process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
    // process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

    printChart(gantt_chart);
    printf("%2d", ctime);

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
    printf("\n\t\tPriority Non premptive Selected.\n No. of processes:");
    scanf("%d", &np);
    struct Process *process = (struct Process *)calloc(np, sizeof(struct Process));

    for (int i = 0; i < np; i++)
    {
        printf("For Process P%d:\n\tArrival time:", i + 1);
        scanf("%d", &process[i].AT);
        printf("\t Burst Time:");
        scanf("%d", &process[i].BT);
        printf("\t Priority:");
        scanf("%d", &process[i].priority);
        process[i].RBT = process[i].BT;
        process[i].id = i;
        process[i].arrived = 0;
        process[i].completed = 0;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    int current_time = 0;
    int total_waiting_time = 0;
    int total_turn_around_time = 0;

    // run the processes
    for (int i = 0; i < np; i++)
    {
        if (current_time < process[i].AT)
        {
            current_time = process[i].AT;
        }

        current_time += overheadTime;
        process[i].FT = current_time + process[i].BT;
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;
        total_waiting_time += process[i].WT;
        total_turn_around_time += process[i].TAT;

        current_time += process[i].BT;
    }

    // PRINT FINAL TABLE and AVERAGES
    float totalTAT = 0, totalWT = 0;

    printf("\n\nProcess | ArrivalTime | BurstTime | Priority | FinishTime | TurnAroundTime | WaitingTime");
    for (int i = 0; i < np; i++)
    {
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;

        totalTAT += process[i].TAT;
        totalWT += process[i].WT;
        printf("\n   P%d\t    %5d\t%5d\t   %5d\t %5d\t      %5d\t     %5d", process[i].id, process[i].AT, process[i].BT, process[i].priority, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms", totalTAT / (float)np);
    printf("\nAverage Waiting Time: %4.3f ms", totalWT / (float)np);
}

void PriorityPremptive()
{
    int np;
    printf("\n\t\tPriority Premptive Selected.\n No. of processes:");
    scanf("%d", &np);
    struct Process *process = (struct Process *)calloc(np, sizeof(struct Process));

    for (int i = 0; i < np; i++)
    {
        printf("For Process P%d:\n\tArrival time:", i + 1);
        scanf("%d", &process[i].AT);
        printf("\t Burst Time:");
        scanf("%d", &process[i].BT);
        printf("\t Priority:");
        scanf("%d", &process[i].priority);
        process[i].RBT = process[i].BT;
        process[i].id = i;
        process[i].arrived = 0;
        process[i].completed = 0;
    }

    printf("\n\tOver Head Time: ");
    int overheadTime;
    scanf("%d", &overheadTime);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 0;

    struct Node *processQueue = NULL;
    struct ChartNode *gantt_chart = NULL;

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

                    gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime); // new process start in ganttchart
                }
                else if (process[i].priority < process[cprocessIndx].priority)
                {
                    // Push running process to Queue
                    processQueue = priorityInsert(processQueue, process[cprocessIndx]);

                    if (overheadTime != 0) // Process Switching due to PREMPTION
                    {
                        gantt_chart = chartInsert(gantt_chart, -1, ctime); // pushing overhead time to ganttchart
                        ctime += overheadTime;                             // Add overhead
                    }
                    cprocessIndx = i;
                    cprocessRBT = process[cprocessIndx].RBT;

                    gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime);
                }
                else
                {
                    processQueue = priorityInsert(processQueue, process[i]);
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
                if (processCount == np)
                {
                    break;
                }
            }

            if (processQueue == NULL)
            {
                gantt_chart = chartInsert(gantt_chart, -2, ctime);
                // NO PROCESS ZONE
                ctime++;
                cprocessIndx = -1;
                continue;
            }

            else
            {
                if (overheadTime != 0)
                {
                    gantt_chart = chartInsert(gantt_chart, -1, ctime); // pushing overhead time to ganttchart
                    // Add overhead for switching
                    ctime += overheadTime;
                }

                // Give CPU a new Process
                cprocessIndx = getLowestProcess(&processQueue).id;
                // printf("\n%d  and processCount=%d", cprocessIndx, processCount);
                cprocessRBT = process[cprocessIndx].RBT;

                gantt_chart = chartInsert(gantt_chart, cprocessIndx, ctime);
            }
        }

        ctime++;
        cprocessRBT--;
        process[cprocessIndx].RBT--;
    }

    // process[cprocessIndx].FT = ctime - 1 + overheadTime + process[cprocessIndx].RBT;
    // process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
    // process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;

    printChart(gantt_chart);
    printf("%2d", ctime);

    // PRINT FINAL TABLE and AVERAGES
    float totalTAT = 0, totalWT = 0;

    printf("\n\nProcess | ArrivalTime | BurstTime | Priority | FinishTime | TurnAroundTime | WaitingTime");
    for (int i = 0; i < np; i++)
    {
        process[i].TAT = process[i].FT - process[i].AT;
        process[i].WT = process[i].TAT - process[i].BT;

        totalTAT += process[i].TAT;
        totalWT += process[i].WT;
        printf("\n   P%d\t    %5d\t%5d\t   %5d\t %5d\t      %5d\t     %5d", process[i].id, process[i].AT, process[i].BT, process[i].priority, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms", totalTAT / (float)np);
    printf("\nAverage Waiting Time: %4.3f ms", totalWT / (float)np);
}

int getNextIndx(struct Process *p, int length, int prevIndx, int ctime)
{
    int i = prevIndx;
    do
    {
        i = (i + 1) % length;
    } while (!(p[i].completed == 0 && p[i].AT <= ctime));

    return i;
}

void Round_Robin()
{
    int np;
    printf("\n\t\tRound robin Selected.\n No. of processes:");
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

    printf("\n\tTime Quantum: ");
    int time_slice;
    scanf("%d", &time_slice);

    SortByAT(process, np);

    int ctime = 0, cprocessIndx = -1, cprocessRBT = 0, processCount = 0, cSliceCounter = time_slice;
    // printf("\ntime| process | RBT\n----------------------");
    while (processCount != np)
    {
        if (process[0].AT <= ctime)
        {
            if (cprocessIndx == -1)
            { // No process selected
                cprocessIndx = 0;
                cprocessRBT = process[cprocessIndx].RBT;
                processCount++;
            }

            if (cprocessRBT == 0)
            {
                // Process completed.

                // Fill the process data
                process[cprocessIndx].FT = ctime;
                process[cprocessIndx].TAT = process[cprocessIndx].FT - process[cprocessIndx].AT;
                process[cprocessIndx].WT = process[cprocessIndx].TAT - process[cprocessIndx].BT;
                process[cprocessIndx].completed = 1;

                processCount++;

                // switch Process
                ctime += overheadTime;

                cprocessIndx = getNextIndx(process, np, cprocessIndx, ctime);
                cprocessRBT = process[cprocessIndx].RBT;
                cSliceCounter = time_slice;
                // printf("\n----------------------");
            }

            if (cSliceCounter == 0)
            {
                // switch Process
                int nextIndx = getNextIndx(process, np, cprocessIndx, ctime);
                if (nextIndx != cprocessIndx)
                {
                    ctime += overheadTime;

                    cprocessIndx = nextIndx;
                    // printf("\n----------------------");
                }
                cprocessRBT = process[cprocessIndx].RBT;
                cSliceCounter = time_slice;
            }

            cSliceCounter--;
            cprocessRBT--;
            process[cprocessIndx].RBT--;
            // printf("\n %2d |   P%d   |   %2d  ", ctime, process[cprocessIndx].id, process[cprocessIndx].RBT);
        }
        ctime++;
        // printf("\n\t\t\t\t%d", processCount);
    }

    process[cprocessIndx].FT = ctime + process[cprocessIndx].RBT;
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
        printf("\n   P%d\t    %5d\t%5d\t      %5d\t     %5d\t    %5d", process[i].id, process[i].AT, process[i].BT, process[i].FT, process[i].TAT, process[i].WT);
    }
    printf("\nAverage Turn Around Time: %4.3f ms", totalTAT / (float)np);
    printf("\nAverage Waiting Time: %4.3f ms", totalWT / (float)np);
}

void main()
{

    int selection;
    printf("Select Schedular:\n1) First Come First Serve\n2) Shortest Job First(Non-premptive)\n3) Shortest Remaining Time Next (SRTN)\n4)Priority (Non-Premptive)\n5) Priority Premptive\n6)Round Robin");
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
    case 5:
        PriorityPremptive();
        break;
    case 6:
        Round_Robin();
        break;

    default:
        break;
    }
}