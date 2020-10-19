//
// Created by 310165137 on 19/10/2020.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <semaphore.h>
#include "parallelmergesort.h"

signed int *array_to_be_sorted = NULL;
sem_t arrayReady;
sem_t firstHalfReady;
sem_t SecondHalfReady;
sem_t mergeReady;

void merge(int *A,int p,int q,int r)
{
    int n1=q-p+1,n2=r-q;
    int L[n1+1],R[n2+1],i,j,k;
    L[n1]=INT_MAX;
    R[n2]=INT_MAX;

    for (i=0;i<n1;i++)
        L[i]=A[p+i];

    for (j=0;j<n2;j++)
        R[j]=A[q+j+1];

    i=0;j=0;

    for (k=p;k<=r;k++)
    {
        if(L[i]<=R[j])
        {
            A[k]=L[i];
            i++;
        }
        else
        {
            A[k]=R[j];
            j++;
        }
    }
}

void mergeSort(int *A,int p,int r)
{
    if (p<r)
    {
        int q=(p+r)/2;
        mergeSort(A,p,q);
        mergeSort(A,q+1,r);
        merge(A,p,q,r);
    }
}


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
    sem_post(&arrayReady); // For the first half
    sem_post(&arrayReady); // For the second half
}


void *sortSecondHalf(void* args)
{
    sem_wait(&arrayReady);
    unsigned int arraysize = *(int*) args;
    mergeSort(array_to_be_sorted,0,arraysize/2); // Pass starting from 0 to Length - 1 of Array
    sem_post(&firstHalfReady);
}

void *sortFirstHalf(void* args)
{
    sem_wait(&arrayReady);
    unsigned int arraysize = *(int*) args;
    mergeSort(array_to_be_sorted,arraysize/2+1,arraysize-1); // Pass starting from 0 to Length - 1 of Array
    sem_post(&SecondHalfReady);
}

void *mergeTwoHalves(void* args)
{
    sem_wait(&firstHalfReady);
    sem_wait(&SecondHalfReady);

    printf("Merging halves");
    sem_post(&mergeReady);

}

void *printSortedArray(void* args)
{
    sem_post(&mergeReady);
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

    sem_init(&arrayReady, 0, 0);
    sem_init(&firstHalfReady, 0, 0);
    sem_init(&SecondHalfReady, 0, 0);
    sem_init(&mergeReady, 0, 0);


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
