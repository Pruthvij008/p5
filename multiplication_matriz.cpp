// Write a program to implement Parallel matrix-matrix multiplication using OpenMP.
#include <iostream>
#include <omp.h>
#include <chrono>
using namespace std;

#define MAX 100

void sequentialMatrixMultiply(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int m, int p) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < p; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

void parallelMatrixMultiply(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int m, int p) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < p; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

void printMatrix(int M[MAX][MAX], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            cout << M[i][j] << " ";
        cout << endl;
    }
}

int main() {
    int n, m, p;
    int A[MAX][MAX], B[MAX][MAX], C_seq[MAX][MAX] = {}, C_par[MAX][MAX] = {};

    cout << "Enter dimensions (A: n x p, B: p x m)\n";
    cout << "Enter n: ";
    cin >> n;
    cout << "Enter p: ";
    cin >> p;
    cout << "Enter m: ";
    cin >> m;

    cout << "Enter matrix A elements:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < p; ++j) {
            cout << "A[" << i << "][" << j << "]: ";
            cin >> A[i][j];
        }

    cout << "Enter matrix B elements:\n";
    for (int i = 0; i < p; ++i)
        for (int j = 0; j < m; ++j) {
            cout << "B[" << i << "][" << j << "]: ";
            cin >> B[i][j];
        }

    auto startSeq = chrono::high_resolution_clock::now();
    sequentialMatrixMultiply(A, B, C_seq, n, m, p);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    auto startPar = chrono::high_resolution_clock::now();
    parallelMatrixMultiply(A, B, C_par, n, m, p);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Matrix-Matrix Multiplication Output -----\n";
    printMatrix(C_seq, n, m);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Matrix Multiplication Output -----\n";
    printMatrix(C_par, n, m);
    cout << "Time Taken: " << durationPar.count() << " milliseconds\n";

    return 0;
}
