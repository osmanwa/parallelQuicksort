#include <ctime>
#include <vector>
#include <random>
#include <iostream>

#define MAX_LIST_SIZE 100000000
#define MIN_LIST_SIZE 1000000
#define NUM_TRIALS 3
#define STEP 1000000
#define THRESHOLD 1000000

using namespace std;

vector<int> list;
int listSize;
void printList(vector<int> list);
void printRunTime(double runTime);
vector<int> fillList();
void doSerialQuicksort(int left, int right);
void doParallelQuicksort(int left, int right);

int main(int argc, char* argv[]) {
    clock_t startTime;
    double runTime, sum, avgRunTime;

    cout << "serial" << endl;
    for (listSize = MIN_LIST_SIZE; listSize <= MAX_LIST_SIZE; listSize += STEP) {
        for (int trialNum = 0; trialNum < NUM_TRIALS; trialNum++) {
            list = fillList();
            startTime = clock();
            doSerialQuicksort(0, listSize - 1);
            runTime = (double) (clock() - startTime);
            sum += runTime;
        }
        avgRunTime =  sum / NUM_TRIALS;
        cout << listSize << "\t";
        printRunTime(avgRunTime);
        cout << endl;
    }

    sum = 0;

    cout << "parallel" << endl;
    for (listSize = MIN_LIST_SIZE; listSize <= MAX_LIST_SIZE; listSize += STEP) {
        for (int trialNum = 0; trialNum < NUM_TRIALS; trialNum++) {
            list = fillList();
            startTime = clock();
            #pragma omp single nowait
            doParallelQuicksort(0, listSize - 1);
            runTime = (double) (clock() - startTime);
            sum += runTime;
        }
        avgRunTime =  sum / NUM_TRIALS;
        cout << listSize << "\t";
        printRunTime(avgRunTime);
        cout << endl;
//    }

    return 0;
}


// use task decomposition to parallelize quicksort
void doParallelQuicksort(int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = list[(left + right) / 2];
    while (i <= j) {
        while (list[i] < pivot)
            i++;
        while (list[j] > pivot)
            j--;
        if (i <= j) {
            tmp = list[i];
            list[i] = list[j];
            list[j] = tmp;
            i++;
            j--;
        }
    }
    if (right-left < THRESHOLD) {
        doSerialQuicksort(left, j);
        doSerialQuicksort(i, right);

    }
    else {
        #pragma omp task
        {
            doParallelQuicksort(left, j);
        }
        #pragma omp task
        {
            doParallelQuicksort(i, right);
        }
    }

}



// run sequential version of quicksort
void doSerialQuicksort(int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = list[(left + right) / 2];
    while (i <= j) {
        while (list[i] < pivot)
            i++;
        while (list[j] > pivot)
            j--;
        if (i <= j) {
            tmp = list[i];
            list[i] = list[j];
            list[j] = tmp;
            i++;
            j--;
        }
    }
    if (left < j) {
        doSerialQuicksort(left, j);
    }
    if (i< right) {
        doSerialQuicksort(i, right);
    }
}

// generate list of random integers
vector<int> fillList(){
    random_device rd;
    mt19937 mt(rd());
    vector<int> list;
    uniform_int_distribution<int> dist(1, listSize);
    for(int i = 0; i < listSize; i++){
        list.push_back(dist(mt));
    }
    return list;
}

void printRunTime(double runTime) {
    cout << (runTime / CLOCKS_PER_SEC) << "\t";
}

void printList(vector<int> list) {
    for (int i = 0; i < MAX_LIST_SIZE; i++) {
        cout << list[i] << "\t";
    }
    cout << endl;
}
