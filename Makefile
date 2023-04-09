main.exe: main.c LongNumber.o pq.o
	gcc -std=c99 main.c LongNumber.o pq.o -o main.exe

LongNumber.o: LongNumber.h LongNumber.c
	gcc -std=c99 LongNumber.c -c

pq.o: pq.h pq.c
	gcc -std=c99 pq.c -c
