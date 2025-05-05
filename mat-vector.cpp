/*Write a program to implement Parallel matrix vector multiplication using OpenMp.*/

// File: parallel_matrix_vector_multiplication.cpp

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

void sequentialMultiply(vector<vector<int>>& A, vector<int>& X, vector<int>& result, int n, int m) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < m; j++) {
            result[i] += A[i][j] * X[j];
        }
    }
}

void parallelMultiply(vector<vector<int>>& A, vector<int>& X, vector<int>& result, int n, int m) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < m; j++) {
            result[i] += A[i][j] * X[j];
        }
    }
}

int main() {
    int n, m;
    cout << "Enter the number of rows in matrix (n): ";
    cin >> n;
    cout << "Enter the number of columns in matrix (m): ";
    cin >> m;

    vector<vector<int>> A(n, vector<int>(m));
    vector<int> X(m), result_seq(n), result_par(n);

    srand(time(0));

    // cout << "Generating random matrix and vector...\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            A[i][j] = rand() % 100;

    for (int i = 0; i < m; i++)
        X[i] = rand() % 100;

    auto start_seq = high_resolution_clock::now();
    sequentialMultiply(A, X, result_seq, n, m);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);

    auto start_par = high_resolution_clock::now();
    parallelMultiply(A, X, result_par, n, m);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);

    cout << "\nTime taken by Sequential Matrix-Vector Multiplication: " << duration_seq.count() << " milliseconds\n";
    cout << "Time taken by Parallel Matrix-Vector Multiplication:   " << duration_par.count() << " milliseconds\n";

    return 0;
}


// ### ✅ Compilation (Linux/macOS/WSL)
// ```bash
// g++ -fopenmp matrix_vector_multiplication_omp.cpp -o matrix_vector_multiplication_omp
// ./matrix_vector_multiplication_omp
// ```

// ---

// ### 📌 Sample Input / Output

// Enter the number of rows in matrix (n): 50000
// Enter the number of columns in matrix (m): 1000

// Time taken by Sequential Matrix-Vector Multiplication: 445 milliseconds
// Time taken by Parallel Matrix-Vector Multiplication:   532 milliseconds


// === Code Execution Successful ===

// ### 📘 **Viva Questions & Short Answers**

// 1. **Q: What is Matrix-Vector Multiplication?**  
//    **A:** Matrix-Vector multiplication involves multiplying a matrix with a vector, where each element in the resulting vector is the dot product of a row of the matrix and the vector.

// 2. **Q: What is the time complexity of Matrix-Vector Multiplication?**  
//    **A:** The time complexity is O(n²) for a square matrix of size `n x n` and a vector of size `n`.

// 3. **Q: Can Matrix-Vector Multiplication be parallelized?**  
//    **A:** Yes, since the multiplication of different rows of the matrix with the vector is independent, it can be parallelized.

// 4. **Q: What does the directive `#pragma omp parallel for` do?**  
//    **A:** It instructs the compiler to parallelize the following loop, dividing its iterations across multiple threads.

// 5. **Q: What is OpenMP?**  
//    **A:** OpenMP is a parallel programming model used for multi-threaded programming on shared-memory systems. It allows you to add parallelism to C, C++, and Fortran programs through compiler directives.

// 6. **Q: Why does parallel matrix-vector multiplication improve performance?**  
//    **A:** It reduces the execution time by performing computations on multiple cores simultaneously.

// 7. **Q: How is parallelism implemented in this program?**  
//    **A:** By using the `#pragma omp parallel for` directive, each row of the matrix is multiplied by the vector in parallel.

// 8. **Q: What does the `printVector` function do?**  
//    **A:** It prints the elements of the result vector to the console.

// 9. **Q: What is the difference between sequential and parallel execution in this program?**  
//    **A:** The sequential version performs the matrix-vector multiplication one row at a time, while the parallel version uses multiple threads to compute rows simultaneously.

// 10. **Q: How do you measure performance in this program?**  
//     **A:** The program measures execution time for both sequential and parallel versions using the `chrono` library and outputs the result in microseconds.
