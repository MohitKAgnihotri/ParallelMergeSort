//
// Created by 310165137 on 19/10/2020.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "parallelmergesort.h"



signed int *array_to_be_sorted = NULL;

void *createArray(void *args)
{
    unsigned int arraysize = (unsigned int ) args;
    array_to_be_sorted = (signed int *) malloc(sizeof (int) * arraysize);
    printf("%s", "Random generated integers are in the following order \n");
    for (signed int i = 0; i < arraysize; i++)
    {
        array_to_be_sorted[i] = (rand() % (UPPER_RANGE - LOWER_RANGE + 1)) + LOWER_RANGE;
        printf("%d \t ", array_to_be_sorted[i]);
    }
}


void *sortSecondHalf(void* args)
{

}


void *mergeTwoHalves(void* args)
{

}

void *printSortedArray(void* args)
{

}

int main(int argc, char *argv[])
{
    pthread_t  th1, th2, th3, th4, th5;

    if (argc < 2)
    {
        printf("Array Size is missing; Correct Usage: %s <Size>", argv[0]);
    }

    srand(time(0));

    int rc = pthread_create(&th1, NULL, createArray, (void *)argv[1]);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

}
