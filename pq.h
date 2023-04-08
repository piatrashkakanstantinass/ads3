// 1brka8845
#ifndef PQ_H
#define PQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100000

typedef struct
{
    int count;
    int *priority;
    char **items;
} PriorityQueue;

PriorityQueue *createPriorityQueue();
PriorityQueue *clonePriorityQueue(PriorityQueue *adt);
void priorityQueueMakeEmpty(PriorityQueue *adt);
int priorityQueueCount(PriorityQueue *adt);
int priorityQueueIsEmpty(PriorityQueue *adt);
int priorityQueueIsFull(PriorityQueue *adt);
char *priorityQueueToString(PriorityQueue *adt);
char* priorityQueuePeek(PriorityQueue *adt);
void priorityQueueInsert(PriorityQueue *adt, char *item, int priority);
char *priorityQueueExtract(PriorityQueue *adt);
PriorityQueue *donePriorityQueue(PriorityQueue *adt);
#endif
