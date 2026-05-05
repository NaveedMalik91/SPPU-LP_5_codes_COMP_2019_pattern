#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

/* ---------- SEQUENTIAL ---------- */

void seq_min(int arr[], int n) {
    int min_val = INT_MAX;
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++)
        if (arr[i] < min_val) min_val = arr[i];

    double end = omp_get_wtime();

    cout << "Sequential Min: " << min_val 
         << " | Time: " << (end - start) << "\n";
}

void seq_max(int arr[], int n) {
    int max_val = INT_MIN;
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++)
        if (arr[i] > max_val) max_val = arr[i];

    double end = omp_get_wtime();

    cout << "Sequential Max: " << max_val 
         << " | Time: " << (end - start) << "\n";
}

void seq_sum(int arr[], int n) {
    int sum = 0;
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++)
        sum += arr[i];

    double end = omp_get_wtime();

    cout << "Sequential Sum: " << sum 
         << " | Time: " << (end - start) << "\n";
}

void seq_avg(int arr[], int n) {
    int sum = 0;
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++)
        sum += arr[i];

    double avg = (double)sum / n;
    double end = omp_get_wtime();

    cout << "Sequential Avg: " << avg 
         << " | Time: " << (end - start) << "\n";
}

/* ---------- PARALLEL ---------- */

void par_min(int arr[], int n) {
    int min_val = INT_MAX;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < n; i++)
        if (arr[i] < min_val) min_val = arr[i];

    double end = omp_get_wtime();

    cout << "Parallel Min: " << min_val 
         << " | Time: " << (end - start) << "\n";
}

void par_max(int arr[], int n) {
    int max_val = INT_MIN;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < n; i++)
        if (arr[i] > max_val) max_val = arr[i];

    double end = omp_get_wtime();

    cout << "Parallel Max: " << max_val 
         << " | Time: " << (end - start) << "\n";
}

void par_sum(int arr[], int n) {
    int sum = 0;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
        sum += arr[i];

    double end = omp_get_wtime();

    cout << "Parallel Sum: " << sum 
         << " | Time: " << (end - start) << "\n";
}

void par_avg(int arr[], int n) {
    int sum = 0;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
        sum += arr[i];

    double avg = (double)sum / n;
    double end = omp_get_wtime();

    cout << "Parallel Avg: " << avg 
         << " | Time: " << (end - start) << "\n";
}

/* ---------- MAIN ---------- */

int main() {
    int n;
    cout << "Enter size: ";
    cin >> n;

    int *arr = new int[n];

    // Use random values for fair timing
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000;

    cout << "\n--- Sequential ---\n";
    seq_min(arr, n);
    seq_max(arr, n);
    seq_sum(arr, n);
    seq_avg(arr, n);

    cout << "\n--- Parallel ---\n";
    par_min(arr, n);
    par_max(arr, n);
    par_sum(arr, n);
    par_avg(arr, n);

    delete[] arr;
    return 0;
}