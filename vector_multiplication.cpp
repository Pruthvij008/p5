// Write a program to implement Parallel matrix-vector multiplication using OpenMP.
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
using namespace std;

#define MAX 100

void sequentialMatrixVectorMultiply(int A[MAX][MAX], const vector<int>& x, vector<int>& result, int n, int m) {
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < m; ++j)
            result[i] += A[i][j] * x[j];
    }
}

void parallelMatrixVectorMultiply(int A[MAX][MAX], const vector<int>& x, vector<int>& result, int n, int m) {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < m; ++j)
            result[i] += A[i][j] * x[j];
    }
}

void printVector(const vector<int>& result) {
    for (auto val : result)
        cout << val << " ";
    cout << endl;
}

int main() {
    int n, m;

    cout << "Enter dimensions (Matrix A: n x m, Vector x: m x 1)\n";
    cout << "Enter n: ";
    cin >> n;
    cout << "Enter m: ";
    cin >> m;

    int A[MAX][MAX];  // Matrix A is still an array
    vector<int> x(m); // Vector x is a vector of size m
    vector<int> resultSeq(n, 0), resultPar(n, 0); // Result vectors for sequential and parallel multiplication

    cout << "Enter matrix A elements:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            cout << "A[" << i << "][" << j << "]: ";
            cin >> A[i][j];
        }

    cout << "Enter vector x elements:\n";
    for (int i = 0; i < m; ++i) {
        cout << "x[" << i << "]: ";
        cin >> x[i];
    }

    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMatrixVectorMultiply(A, x, resultSeq, n, m);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    auto startPar = chrono::high_resolution_clock::now();
    parallelMatrixVectorMultiply(A, x, resultPar, n, m);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Matrix-Vector Multiplication Output -----\n";
    printVector(resultSeq);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Vector Multiplication Output -----\n";
    printVector(resultPar);
    cout << "Time Taken: " << durationPar.count() << " milliseconds\n";

    return 0;
}
