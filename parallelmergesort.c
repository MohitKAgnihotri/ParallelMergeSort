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
    unsigned int arraysize = *(int*) args;
    array_to_be_sorted = (signed int *) malloc(sizeof (int) * arraysize);
    printf("%s", "Random generated integers are in the following order \n");
    for (signed int i = 0; i < arraysize; i++)
    {
        array_to_be_sorted[i] = (rand() % (UPPER_RANGE - LOWER_RANGE + 1)) + LOWER_RANGE;
        printf("%d \t ", array_to_be_sorted[i]);
    }
    printf("\n");
}


void *sortSecondHalf(void* args)
{

}

void *sortFirstHalf(void* args)
{

}

void *mergeTwoHalves(void* args)
{

}

void *printSortedArray(void* args)
{
    unsigned int arraysize = *(int*) args;
    printf("%s", "The element of array have been sorted as follows: \n");
    for (signed int i = 0; i < arraysize; i++)
    {
        printf("%d \t ", array_to_be_sorted[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    pthread_t  th1, th2, th3, th4, th5;
    void *status;
    int array_size;

    if (argc < 2)
    {
        printf("Array Size is missing; Correct Usage: %s <Size>", argv[0]);
    }

    array_size = atoi(argv[1]);
    srand(time(0));

    int rc = pthread_create(&th1, NULL, createArray, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th2, NULL, sortFirstHalf, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th2, NULL, sortSecondHalf, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th2, NULL, mergeTwoHalves, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th2, NULL, printSortedArray, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    pthread_join(th1, &status);
    pthread_join(th2, &status);
    pthread_join(th3, &status);
    pthread_join(th4, &status);
    pthread_join(th5, &status);
}
