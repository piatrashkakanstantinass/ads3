#include "pq.h"

PriorityQueue *createPriorityQueue()
{
    PriorityQueue *adt = malloc(sizeof(PriorityQueue));
    adt->count = 0;
    adt->items = malloc(MAX_SIZE * sizeof(char *));
    adt->priority = malloc(MAX_SIZE * sizeof(int));
    return adt;
}

PriorityQueue *priorityQueueClone(PriorityQueue *adt)
{
    PriorityQueue *new_adt = malloc(sizeof(PriorityQueue));
    new_adt->count = adt->count;
    new_adt->items = malloc(MAX_SIZE * sizeof(char *));
    for (int i = 0; i < adt->count; i++)
    {
        new_adt->items[i] = adt->items[i];
    }
    return new_adt;
}

void priorityQueueMakeEmpty(PriorityQueue *adt)
{
    if (!priorityQueueIsEmpty(adt))
    {
        for (int i = 0; i < adt->count; i++)
        {
            free(adt->items[i]);
        }
        adt->count = 0;
        free(adt->priority);
    }
}

int priorityQueueCount(PriorityQueue *adt)
{
    return adt->count;
}

int priorityQueueIsEmpty(PriorityQueue *adt)
{
    return adt->count == 0;
}

int priorityQueueIsFull(PriorityQueue *adt)
{
    return adt->count == MAX_SIZE;
}

char *priorityQueueToString(PriorityQueue *adt)
{
    const int MAX_LENGTH = 10000;
    char *str = malloc(MAX_LENGTH * sizeof(char));
    str[0] = '\0';
    strcat(str, "ADT:\n");
    if (adt->count > MAX_SIZE)
    {
        strcat(str, "There are too many items to display\n");
    }
    else
    {
        for (int i = 0; i < adt->count; i++)
        {
            strcat(str, adt->items[i]);
            strcat(str, "\n");
        }
    }
    return str;
}
// Gauna pirmo eilės elemento duomenis, neišimant jo
char *priorityQueuePeek(PriorityQueue *adt)
{
    if (priorityQueueIsEmpty(adt))
    {
        printf("\nPriority queue is empty!\n");
        return NULL;
    }
    return adt->items[0];
}

// Inserts a new item into the priority queue, with the given priority
void priorityQueueInsert(PriorityQueue *adt, char *item, int priority)
{
    if (!priorityQueueIsFull(adt))
    {
        // Find the index where the new item should be inserted based on its priority
        int index = 0;
        for (int i = 0; i < adt->count; ++i)
        {
            if (index < adt->count && priority >= adt->priority[i])
            {
                index++;
            }
        }

        // Shift all items with higher priority to the right
        for (int i = adt->count - 1; i >= index; --i)
        {
            adt->items[i + 1] = adt->items[i];
            adt->priority[i + 1] = adt->priority[i];
        }

        // Insert the new item
        // char *str = malloc(MAX_SIZE * sizeof(char));
        // sprintf(str, "%s %d", item, priority);
        adt->items[index] = item;
        adt->priority[index] = priority;
        adt->count++;
    }
    else
    {
        printf("ADT is full\n");
    }
}

// Removes and returns the item with the highest priority from the priority queue
char *priorityQueueExtract(PriorityQueue *adt)
{
    if (priorityQueueIsEmpty(adt))
    {
        printf("Priority queue is empty!\n");
        return NULL;
    }

    int index = adt->count - 1;
    int max_priority = adt->priority[index];
    while (--index >= 0 && adt->priority[index] == max_priority)
      ;
    ++index;
    // Remove and return the item with the highest priority
    char *item = adt->items[index];
    adt->count--;
    for (; index < adt->count; ++index) {
      adt->items[index] = adt->items[index + 1];
      adt->priority[index] = adt->priority[index + 1];
    }
    return item;
}
PriorityQueue *donePriorityQueue(PriorityQueue *adt) // destroy ADT
{
    for (int i = 0; i < adt->count; ++i) {
      free(adt->items[i]);
    }
    free(adt->items);
    free(adt);
    return adt;
}
