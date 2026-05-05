#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <iomanip>

using namespace std;

void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

void parallelBubble(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
        {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        }
        else
        {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        }
    }
}

void merge(int arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelMerge(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMerge(arr, l, m);

            #pragma omp section
            parallelMerge(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main()
{
    cout << fixed << setprecision(6);

    int n;
    cout << "Enter size of the array (Total elements): ";
    cin >> n;

    int *arr = new int[n];
    int *backup = new int[n];

    for (int i = 0; i < n; i++)
        backup[i] = rand();

    double start, end;

    // Sequential Bubble
    for (int i = 0; i < n; i++) arr[i] = backup[i];
    start = omp_get_wtime();
    bubbleSort(arr, n);
    end = omp_get_wtime();
    double t1 = end - start;

    // Parallel Bubble
    for (int i = 0; i < n; i++) arr[i] = backup[i];
    start = omp_get_wtime();
    parallelBubble(arr, n);
    end = omp_get_wtime();
    double t2 = end - start;

    // Sequential Merge
    for (int i = 0; i < n; i++) arr[i] = backup[i];
    start = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    end = omp_get_wtime();
    double t3 = end - start;

    // Parallel Merge
    for (int i = 0; i < n; i++) arr[i] = backup[i];
    start = omp_get_wtime();
    parallelMerge(arr, 0, n - 1);
    end = omp_get_wtime();
    double t4 = end - start;

    cout << "\nExecution Time:\n";
    cout << "Sequential Bubble: " << t1 << " sec\n";
    cout << "Parallel Bubble:   " << t2 << " sec\n";
    cout << "Sequential Merge:  " << t3 << " sec\n";
    cout << "Parallel Merge:    " << t4 << " sec\n";

    delete[] arr;
    delete[] backup;

    return 0;
}