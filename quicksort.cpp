#include "quicksort.h"

// runs 6 trials for serial with given pivot position; prints time in ms tab-separated
void runSerialTrial(PivotPosition pivot, int numTrials) {
    for (int i = 0; i < numTrials; i++) {
        int *list = fillList();
        clock_t startTime = clock();
        doSequentialQuicksort(list, 0, LIST_SIZE - 1, pivot);
        double runTime = (double)(clock() - startTime);
        printRunTime(runTime);
    }
}

void printRunTime(double runTime) {
    cout << (runTime / CLOCKS_PER_MS) << "\t";
}

// runs 6 trials with given decomposition type, pivot position and number of threads. prints time in ms tab-separated
void runParallelTrial(DecompositionType decomposition, PivotPosition pivot, int numThreads, int numTrials) {
    omp_set_num_threads(numThreads);
    for (int i = 0; i < numTrials; i++) {
        doParallelQuicksort(decomposition, pivot, numThreads);
    }
}

// runs quicksort algorithm based on input parameters
void doParallelQuicksort(DecompositionType decomposition, PivotPosition position, int numThreads){
    int *list = fillList();
    clock_t startTime;
    double runTime;
    switch (decomposition) {
        case data:
            startTime = clock();
            doDataSort(list, position, numThreads);
            runTime = startTime = clock();
            break;
        case task:
            startTime = clock();
            doTaskSort(list, 0, LIST_SIZE - 1, position);
            runTime = startTime = clock();
            break;
        case combined:
            startTime = clock();
            doCombinedSort(list, 0, LIST_SIZE - 1, position, numThreads);
            runTime = startTime = clock();
            break;
    }
    printRunTime(runTime);
}

// prints out a space-divided array and ends with a newline
// use for testing
void printList(int* list) {
    for(int i = 0; i < LIST_SIZE; i++){
        cout << list[i] << " ";
    }
    cout << endl;
}

// generate list of random integers
int *fillList(){
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1, LIST_SIZE);
    auto *list = new int[LIST_SIZE];
    for(int i = 0; i < LIST_SIZE; i++){
        list[i] = dist(mt);
    }
    return list;
}

// Swap two elements in array
void swap(int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

// choose first, center or last index of the list as pivot for quicksort
int choosePivot(int i, int j, PivotPosition position){
    switch(position) {
        case first:
            return i;
        case middle:
            return (i + j) / 2;
        case last:
            return j;
    }
}

// use task decomposition to parallelize quicksort
void doTaskSort(int* list, int m, int n, PivotPosition position){
    int key,i, j, k;
    if(m < n){
        k = choosePivot(m, n, position);
        swap(&list[m], &list[k]);
        key = list[m];
        i = m+1;
        j = n;
        while(i <= j){
            while((i <= n) && (list[i] <= key))
                i++;
            while((j >= m) && (list[j] > key))
                j--;
            if(i < j)
                swap(&list[i], &list[j]);
        }
        swap(&list[m], &list[j]);
        #pragma omp parallel sections shared(m, j, n)
        {
            #pragma omp section
            {
                doTaskSort(list, m, j-1, position); // sort to the left of the pivot
            }
            #pragma omp section
            {
                doTaskSort(list, j+1, n, position); // sort to the right of the pivot
            }
        }
    }
}

// use data decomposition to parallelize quicksort
void doDataSort(int* list, PivotPosition position, int numThreads){
    int groupSize = LIST_SIZE / numThreads;
    #pragma omp parallel for
    for (int groupNum = 0; groupNum < numThreads; groupNum++) {  // move through each subgroup
        int start = groupNum * groupSize;           // first element of sublist
        int end = groupNum * groupSize + groupSize - 1; // last element of sublist
        doSequentialQuicksort(list, start, end, position);
    }
    doSequentialQuicksort(list, 0, LIST_SIZE-1, position);
}

// run sequential version of quicksort
void doSequentialQuicksort(int* list, int m, int n, PivotPosition position) {
    int key, i, j, k;
    if(m < n){
        k = choosePivot(m, n, position);
        swap(&list[m], &list[k]);
        key = list[m];
        i = m+1;
        j = n;
        while(i <= j){
            while((i <= n) && (list[i] <= key))
                i++;
            while((j >= m) && (list[j] > key))
                j--;
            if(i < j)
                swap(&list[i], &list[j]);
        }
        swap(&list[m], &list[j]);
        doSequentialQuicksort(list, m, j-1, position); // sort to the left of the pivot
        doSequentialQuicksort(list, j+1, n, position); // sort to the right of the pivot
    }
}

// use data and task decomposition to parallelize quicksort
void doCombinedSort(int* list, int m, int n, PivotPosition position, int numThreads){
    int groupSize = LIST_SIZE / numThreads;
    #pragma omp parallel for
    for (int groupNum = 0; groupNum < numThreads; groupNum++) {
        int start = groupNum * groupSize;
        int end = groupNum * groupSize + groupSize - 1;
        doTaskSort(list, start, end, position);
    }
    doTaskSort(list, 0, LIST_SIZE - 1, position);
}
