//
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "parallelmergesort.h"

/* Global Variables */
signed int *array_to_be_sorted = NULL;
sem_t arrayReady;
sem_t firstHalfReady;
sem_t SecondHalfReady;
sem_t mergeReady;
pthread_t  th1, th2, th3, th4, th5;

// merge function for merging two parts
void merge(int low, int mid, int high)
{

    // n1 is size of left part and n2 is size of right part
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    int i;
    int j;

    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = array_to_be_sorted[i + low];

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = array_to_be_sorted[i + mid + 1];

    int k = low;

    i = j = 0;

    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            array_to_be_sorted[k++] = left[i++];
        else
            array_to_be_sorted[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1)
        array_to_be_sorted[k++] = left[i++];

    // insert remaining values from right
    while (j < n2)
        array_to_be_sorted[k++] = right[j++];

    free(left);
    free(right);
}

// merge sort function
void merge_sort(int low, int high)
{
    if (pthread_self() == th2)
    {
        printf ("*%d\t",+array_to_be_sorted[low]);
    }
    else if (pthread_self() == th3)
    {
        printf ("**%d\t",+array_to_be_sorted[low]);
    }
    else
    {
        // empty
    }

    // calculating mid point of array
    int mid = low + (high - low) / 2;

    if (low < high) {
        // calling first half
        merge_sort(low, mid);

        // calling second half
        merge_sort(mid + 1, high);

        // merging the two halves
        merge(low, mid, high);
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
    printf("****************************Thread1 & Thread2 are concurrently sorting the array********************************************** \n");

    sem_post(&arrayReady); // For the first half
    sem_post(&arrayReady); // For the second half
    pthread_exit(0);
}


void *sortSecondHalf(void* args)
{
    sem_wait(&arrayReady);

    unsigned int arraysize = *(int*) args;
    int low = (arraysize-1)/2+1;
    int high = arraysize-1;
    // evaluating mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
    printf("\n");
    sem_post(&SecondHalfReady);
    pthread_exit(0);
}

void *sortFirstHalf(void* args)
{
    sem_wait(&arrayReady);

    unsigned int arraysize = *(int*) args;
    int low = 0;
    int high = (arraysize-1)/2;
    // evaluating mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
    printf("\n");
    sem_post(&firstHalfReady);
    pthread_exit(0);
}

void *mergeTwoHalves(void* args)
{
    sem_wait(&firstHalfReady);
    sem_wait(&SecondHalfReady);

    unsigned int arraysize = *(int*) args;
    int high = arraysize-1;
    merge(0, high / 2, high);

    sem_post(&mergeReady);
    pthread_exit(0);
}

void *printSortedArray(void* args)
{
    sem_wait(&mergeReady);
    unsigned int arraysize = *(int*) args;
    printf("%s", "The element of array have been sorted as follows: \n");

    for (signed int i = 0; i < arraysize; i++)
    {
        printf("%d \t ", array_to_be_sorted[i]);
    }
    printf("\n");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{

    int *status = NULL;
    int array_size;

    if (argc < 2)
    {
        printf("Array Size is missing; Correct Usage: %s <Size>\n", argv[0]);
        return 0;
    }

    array_size = atoi(argv[1]);
    if (array_size <= 0)
    {
        printf("Array Size is missing; Correct Usage: %s <Size>\n", argv[0]);
        return 0;
    }

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

    rc = pthread_create(&th3, NULL, sortSecondHalf, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th4, NULL, mergeTwoHalves, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&th5, NULL, printSortedArray, (void *)&array_size);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    pthread_join(th1, (void **) &status);
    pthread_join(th2, (void **) &status);
    pthread_join(th3, (void **) &status);
    pthread_join(th4, (void **) &status);
    pthread_join(th5, (void **) &status);
    return 0;
}
