main.exe: main.c LongNumber.o pq.o
	cc -std=c99 main.c LongNumber.o pq.o -o main.exe

LongNumber.o: LongNumber.h LongNumber.c
	cc -std=c99 LongNumber.c -c LongNumber.o

pq.o: pq.h pq.c
	cc -std=c99 pq.c -c pq.o
