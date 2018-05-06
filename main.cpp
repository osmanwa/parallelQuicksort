#include "quicksort.h"

int main(int argc, char* argv[]) {
    int numThreads = 8;
    int numTrials = 6;

    cout << "serial" << endl;
    for (int trialNum = 0; trialNum < numTrials; trialNum++)
        runSerialTrial(middle, 1);              // pivot position, numTrials
    cout << endl << endl;

    cout << "parallel" << endl;
    for (int threadNum = 2; threadNum <= numThreads; threadNum+=2){
        cout << threadNum << " threads" << endl;
        for (int trialNum = 0; trialNum < 10; trialNum++) {
            runParallelTrial(data, last, threadNum, 1);    // decomp type, pivot position, numThreads, numTrials
        }
        cout << endl << endl;
    }
    return 0;
}