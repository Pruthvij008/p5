#include <iostream>
#include <vector>
#include <algorithm>  // for swap
#include <omp.h>      // for OpenMP parallelism
#include <chrono>     // for time measurement

using namespace std;

// Partition function
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1; 

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Sequential Quick Sort
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// Parallel Quick Sort
void parallelQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelQuickSort(arr, low, pi - 1);

            #pragma omp section
            parallelQuickSort(arr, pi + 1, high);
        }
    }
}

// Function to print array
void printArray(const vector<int>& arr) {
    for (auto x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), arr_copy(n);

    cout << "Enter the elements:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Element " << i + 1 << ": ";
        cin >> arr[i];
        arr_copy[i] = arr[i];
    }

    // Sequential Quick Sort
    auto startSeq = chrono::high_resolution_clock::now();
    sequentialQuickSort(arr, 0, n - 1);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    // Parallel Quick Sort
    auto startPar = chrono::high_resolution_clock::now();
    parallelQuickSort(arr_copy, 0, n - 1);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Quick Sort Output -----\n";
    cout << "Sorted Array: ";
    printArray(arr);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds" << endl;

    cout << "\n----- Parallel Quick Sort Output -----\n";
    cout << "Sorted Array: ";
    printArray(arr_copy);
    cout << "Time Taken: " << durationPar.count() << " milliseconds" << endl;

    return 0;
}
