#include <stdio.h>
#include <time.h>

#include "LongNumber.h"
#include "pq.h"

#define BUFF_SIZE 1024

#define VIP_PRIORITY 1
#define REGULAR_PRIORITY 0

#define OPENING_HOUR 8
#define CLOSING_HOUR 17
#define SIMULATION_LENGTH 10 // lenght in days
#define RANDOM_TIME_MULTIPLIER 5
#define OVERHOURS_MULTIPLIER 2

#define xstr(a) str(a)
#define str(a) #a

// Parameters
LongNumber A; // frančizė (minimalus kliento sumokamas mokėstis įvykus avarijai)
LongNumber B; // VIP kliento mokamas priedas
int K = 2; // darbuotoju skaicius
int U = 10; // darbuotojo valandinė alga
int T1 = 30; // tikimybė procentais, kad mašiną pristatys VIP klientas
int T2 = 50; // tikimybė procentais, kad mašiną pristatys paprastas klientas

typedef struct {
  int id;
  int vip;
  int hoursLeft;  
} Client;

int readFile(FILE *fp) {
  if (fp == NULL)
    return 1;
  char inputBuffer[BUFF_SIZE + 1];
  fscanf(fp, "%" xstr(BUFF_SIZE) "[0-9]", inputBuffer);
  A = longNumberCreateFromString(inputBuffer);
  return 0;
}

LongNumber run(int with_vip_clients, int debug) {
  LongNumber profit = longNumberCreateFromLL(0);
  PriorityQueue *pq = createPriorityQueue();
  int hour = 0;
  int day = 0;
  int id = 0;
  srand(time(NULL));

  while (day != SIMULATION_LENGTH) {
    if (debug) {
      printf("====================\n");
      printf("Day hour: %d\n", hour);
    }

    if (hour >= OPENING_HOUR && hour <= CLOSING_HOUR && !priorityQueueIsFull(pq)) {
      if (rand() % 100 < T1 && with_vip_clients) { // VIP client
        if (debug) printf("New VIP client (id= %d, ", id);
        Client *client = (Client*)malloc(sizeof(Client));
        client->id = id++;
        client->vip = 1;
        client->hoursLeft = rand() % (RANDOM_TIME_MULTIPLIER * K) + 1;
        if (debug) printf("timeRequired= %d)\n", client->hoursLeft);
        LongNumber sum1 = longNumberAdd(profit, A);
        profit = longNumberAdd(sum1, B);
        longNumberFree(sum1);
        priorityQueueInsert(pq, (void*)client, VIP_PRIORITY);
      } 
      if (rand() % 100 < T2) { // Regular client
        if (debug) printf("New regular client (id= %d, ", id);
        Client *client = (Client*)malloc(sizeof(Client));
        client->vip = 0;
        client->id = id++;
        client->hoursLeft = rand() % (RANDOM_TIME_MULTIPLIER * K) + 1;
        if (debug) printf("timeRequired= %d)\n", client->hoursLeft);
        profit = longNumberAdd(profit, A);
        priorityQueueInsert(pq, (void*)client, REGULAR_PRIORITY);
      } 
    }
    
    int workersLeft = K;
    PriorityQueue* nextPq = createPriorityQueue();
    while (!priorityQueueIsEmpty(pq)) {
      Client *client = (Client*)priorityQueueExtract(pq);   
      int worked = 0;

      if (hour >= OPENING_HOUR && hour <= CLOSING_HOUR && workersLeft) {
        --workersLeft;
        worked = 1;
        LongNumber lU = longNumberCreateFromLL(U);
        profit = longNumberSubtract(profit, lU);
        longNumberFree(lU);
        client->hoursLeft--;
      }

      if (hour > CLOSING_HOUR && client->vip && workersLeft) {
        --workersLeft;
        worked = 1;
        LongNumber lU = longNumberCreateFromLL(U * OVERHOURS_MULTIPLIER);
        profit = longNumberSubtract(profit, lU);
        longNumberFree(lU);
        client->hoursLeft--;
      }

      if (worked && debug && client->hoursLeft > 0)
        printf("Working on car (id= %d, vip= %d, hoursLeft= %d)\n", client->id, client->vip, client->hoursLeft);
      
      if (client->hoursLeft <= 0) {
        if (debug) printf("Client (id= %d, vip= %d) done\n", client->id, client->vip);
        free(client);
      }
      else
        priorityQueueInsert(nextPq, (void*)client, client->vip ? VIP_PRIORITY : REGULAR_PRIORITY);
    }
    donePriorityQueue(pq);
    pq = nextPq;

    if (debug) printf("Profit: %s\n", longNumberConvertToString(profit));

    if (debug) printf("====================\n\n");
    hour = (hour + 1) % 24;
    if (hour == 0) ++day;
  }
  donePriorityQueue(pq);
  return profit;
}

int main(int argc, const char *argv[]) {
  A = longNumberCreateFromLL(200);
  B = longNumberCreateFromLL(100);
  run(1, 1);
  // if (argc < 2) {
  //   printf("Usage: main.exe input_file\n");
  //   return 1;
  // }
  // FILE *fp = fopen(argv[1], "r");
  // if (fp == NULL) {
  //   printf("Cannot open file\n");
  //   return 1;
  // }
  // readFile(fp);
}
