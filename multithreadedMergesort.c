#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct args
{
    int low;
    int high;
};

int *arr;
void merge(int *arr, int low, int mid, int high)
{
    int i = low;
    int j = mid + 1;
    int k = low;
    int b[10];
    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
        {
            b[k] = arr[i];
            i++;
        }
        else
        {
            b[k] = arr[j];
            j++;
        }
        k++;
    }
    if (i > mid)
    {
        while (j <= high)
        {
            b[k] = arr[j];
            j++;
            k++;
        }
    }
    else
    {
        while (i <= mid)
        {
            b[k] = arr[i];
            i++;
            k++;
        }
    }

    for (k = low; k <= high; k++)
    {
        arr[k] = b[k];
    }
}

void *mergeSort(void *arg)
{
    struct args *parameters = (struct args *)arg;
    int low = parameters->low;
    int high = parameters->high;

    if (low < high) //still to divide
    {
        int mid = (low + high) / 2;
        //create thread for left part
        struct args l, r;
        l.low = low;
        l.high = mid;
        r.low = mid + 1;
        r.high = high;
        pthread_t left, right;
        pthread_create(&left, NULL, &mergeSort, &l);
        pthread_create(&right, NULL, &mergeSort, &r);
        pthread_join(left, NULL);
        pthread_join(right, NULL);
        //merge process
        merge(arr, low, mid, high);
    }
}

int main()
{
    /*
    The proceeding part is the process to read and parse the file
    *
    *
    * *
    */
    char *fname = (char *)malloc(20 + 1);
    printf("Enter File Name:\n");
    scanf("%s", fname);
    FILE *fp;
    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("Cannot open the file");
        exit(1);
    }
    int arraySize;
    fscanf(fp, "%d", &arraySize);
    arr=(int *)malloc(arraySize*sizeof(int));
     for(int i=0; i<arraySize; i++)
        fscanf(fp, "%d", &arr[i]);

    // while (fgets(line, 512, fp))
    // {

    //     if (!flag)
    //     {
    //         arraySize = atoi(line);
    //         printf("%d\n", arraySize);
    //         arr = (int *)malloc(arraySize * sizeof(int));
    //         flag++;
    //     }
    //     else
    //     {
    //         int i = 0;
    //         char *token = strtok(line, " ");
    //         while (token != NULL)
    //         {
    //             arr[i] = atoi(token);
    //             token = strtok(NULL, " ");
    //             // printf("%d\n",arr[i]);
    //             i++;
    //         }
    //     }
    // }
    free(fname);
    fclose(fp);
    // for (int i = 0; i < arraySize; i++)
    // {
    //     printf("%d\n", arr[i]);
    // }

    struct args input;
    input.high = arraySize - 1;
    input.low = 0;
    pthread_t main;
    pthread_create(&main, NULL, &mergeSort, &input);
    pthread_join(main, NULL); // main thread to pass parameters
    for (int i = 0; i < arraySize; i++)
    {
        printf("%d\n", arr[i]);
    }
    free(arr);

    return 0;
}