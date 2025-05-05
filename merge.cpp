#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

// Merge two sorted parts
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); k++)
        arr[left + k] = temp[k];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

// Print Array
void printArray(const vector<int>& arr) {
    for (auto x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), arrCopy(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        arrCopy[i] = arr[i];
    }

    // Sequential Merge Sort
    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMergeSort(arr, 0, n - 1);
    auto endSeq = chrono::high_resolution_clock::now();

    // Parallel Merge Sort
    auto startPar = chrono::high_resolution_clock::now();
    parallelMergeSort(arrCopy, 0, n - 1);
    auto endPar = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> timeSeq = endSeq - startSeq;
    chrono::duration<double, milli> timePar = endPar - startPar;

    cout << "\n--- Sequential Merge Sort Output ---\n";
    cout << "Sorted Array: ";
    printArray(arr);
    cout << "Time: " << timeSeq.count() << " ms\n";

    cout << "\n--- Parallel Merge Sort Output ---\n";
    cout << "Sorted Array: ";
    printArray(arrCopy);
    cout << "Time: " << timePar.count() << " ms\n";

    return 0;
}
