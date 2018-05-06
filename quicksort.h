#include <random>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <omp.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)
#define LIST_SIZE 10000000

using namespace std;

enum DecompositionType
{
    data,
    task,
    combined
};

enum PivotPosition
{
    first,
    middle,
    last
};

void printRunTime(double runTime);
void runSerialTrial(PivotPosition pivot, int numTrials);
void runParallelTrial(DecompositionType decomposition, PivotPosition pivot, int numThreads, int numTrials);
void doSequentialQuicksort(int* list, int m, int n, PivotPosition position);
void doParallelQuicksort(DecompositionType decomposition, PivotPosition position, int numThreads);
void doTaskSort(int* list, int m, int n, PivotPosition position);
void doDataSort(int* list, PivotPosition position, int numThreads);
void doCombinedSort(int* list, int m, int n, PivotPosition position, int numThreads);
int *fillList();
void printList(int* list);
