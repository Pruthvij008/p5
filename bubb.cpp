#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

// Sequential Bubble Sort (correct)
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Early exit if sorted
    }
}

// Parallel Bubble Sort (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    #pragma omp parallel
    {
        for (int i = 0; i < n; i++) {
            #pragma omp barrier // Sync before each pass
            #pragma omp for
            for (int j = i % 2; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

void printArray(const vector<int>& arr) {
    for (auto x : arr) cout << x << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    
    vector<int> arr(n), arr_copy(n);
    
    // Fill with random values (for testing)
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
        arr_copy[i] = arr[i];
    }
    
    // Sequential Sort
    auto startSeq = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;
    
    // Parallel Sort
    auto startPar = chrono::high_resolution_clock::now();
    parallelBubbleSort(arr_copy);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;
    
    // Output Results
    cout << "\n----- Sequential Bubble Sort -----\n";
    cout << "Time Taken: " << durationSeq.count() << " ms\n";
    
    cout << "\n----- Parallel Bubble Sort -----\n";
    cout << "Time Taken: " << durationPar.count() << " ms\n";
    
    // Verify correctness
    bool isSorted = true;
    for (int i = 0; i < n - 1; i++) {
        if (arr_copy[i] > arr_copy[i + 1]) {
            isSorted = false;
            break;
        }
    }
    cout << "\nArray is " << (isSorted ? "sorted correctly." : "NOT sorted correctly!") << endl;
    
    return 0;
}