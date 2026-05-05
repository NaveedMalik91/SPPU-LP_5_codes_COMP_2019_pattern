#include <bits/stdc++.h>
#include <cstdlib>
#include <omp.h>
#include <iomanip>

using namespace std;

//----------------Sequential Bubble sort--------------
void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

//--------------Parallel Bubble sort (Even-odd index pairs compared)-------------
void parallelBubble(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        // Even phase index
        //e.g. (0,1),(2,3),(4,5)...
        if (i % 2 == 0)
        {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        }

        //Odd phase index
        //e.g. (1,2),(3,4),(5,6)....
        else
        {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2)
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
        }
    }
}




//-------------Common merege function------------
void mergeArray(int arr[], int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Temporary arrays for left and right parts
    int *leftArr = new int[leftSize];
    int *rightArr = new int[rightSize];

    // Copy data into temporary arrays
    for (int i = 0; i < leftSize; i++)
        leftArr[i] = arr[left + i];

    for (int j = 0; j < rightSize; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0;          // pointer for leftArr
    int j = 0;          // pointer for rightArr
    int current = left; // pointer for original array

    // Merge both arrays into original array
    while (i < leftSize && j < rightSize)
    {
        if (leftArr[i] <= rightArr[j])
            arr[current++] = leftArr[i++];
        else
            arr[current++] = rightArr[j++];
    }

    //Copy remaining elements (if any)
    while (i < leftSize)
        arr[current++] = leftArr[i++];

    while (j < rightSize)
        arr[current++] = rightArr[j++];

    delete[] leftArr;
    delete[] rightArr;
}


//---------------Sequential merge sort-------------
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        mergeArray(arr, l, m, r);
    }
}


//----------------Parallel merge sort----------------
void parallelMergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

        #pragma omp parallel sections
        {
            // Thread 1 → left half
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            // Thread 2 → right half
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }

        // Merge after both halves are sorted
        mergeArray(arr, left, mid, right);
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
    parallelMergeSort(arr, 0, n - 1);
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