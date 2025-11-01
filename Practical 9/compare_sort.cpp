#include <iostream>
#include <algorithm>   // for sort()
#include <cstdlib>     // for qsort()
#include <ctime>       // for clock()
#include <vector>
#include <fstream>

using namespace std;

// Custom Bubble Sort
void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// Comparison function for qsort()
int compareInts(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}
int main() {
    int n = 10000;
    vector<int> data(n);

    srand(time(0));
    for (int i = 0; i < n; i++)
        data[i] = rand() % 100000;

    vector<int> data_stl = data;
    vector<int> data_qsort = data;
    vector<int> data_custom = data;

    clock_t start, end;

    // STL sort
    start = clock();
    sort(data_stl.begin(), data_stl.end());
    end = clock();
    double time_stl = double(end - start) / CLOCKS_PER_SEC;

    // qsort
    start = clock();
    qsort(&data_qsort[0], n, sizeof(int), compareInts);
    end = clock();
    double time_qsort = double(end - start) / CLOCKS_PER_SEC;

    // Bubble sort
    start = clock();
    bubbleSort(data_custom);
    end = clock();
    double time_custom = double(end - start) / CLOCKS_PER_SEC;

    // Print to console
    cout << "STL sort() time: " << time_stl << " seconds" << endl;
    cout << "qsort() time: " << time_qsort << " seconds" << endl;
    cout << "Custom Bubble Sort time: " << time_custom << " seconds" << endl;

    // Save results to file
    ofstream fout("sort_times.txt");
    fout << "STL_sort " << time_stl << endl;
    fout << "qsort " << time_qsort << endl;
    fout << "BubbleSort " << time_custom << endl;
    fout.close();

    return 0;
}
