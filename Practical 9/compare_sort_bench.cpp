// compare_sort_bench.cpp
// Compile: g++ compare_sort_bench.cpp -O2 -std=c++17 -o compare_sort_bench
// If your system supports parallel execution policies, compile with -std=c++17 (or c++20).

#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <random>
#include <unistd.h> // getpagesize()
#ifdef __has_include
# if __has_include(<execution>)
#  include <execution>
#  define HAVE_EXECUTION 1
# else
#  define HAVE_EXECUTION 0
# endif
#else
# define HAVE_EXECUTION 0
#endif

using namespace std;
using ns = std::chrono::nanoseconds;
using hrclock = std::chrono::high_resolution_clock;

// --- Utility: get approximate resident memory usage in KB ---
long getMemoryUsageKB() {
    std::ifstream statm("/proc/self/statm");
    if (!statm.is_open()) return -1;
    long sizePages=0, residentPages=0;
    statm >> sizePages >> residentPages;
    long pageSize = sysconf(_SC_PAGESIZE);
    long kb = (residentPages * pageSize) / 1024;
    return kb;
}

// --- Bubble Sort (O(n^2)) ---
void bubbleSort(vector<int> &arr) {
    size_t n = arr.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < n - i; ++j) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// --- Merge Sort (O(n log n)) ---
void merge(vector<int> &a, int l, int m, int r, vector<int> &tmp) {
    int i = l, j = m+1, k = l;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++]; else tmp[k++] = a[j++];
    }
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int x = l; x <= r; ++x) a[x] = tmp[x];
}
void mergeSortRec(vector<int> &a, int l, int r, vector<int> &tmp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortRec(a,l,m,tmp);
    mergeSortRec(a,m+1,r,tmp);
    merge(a,l,m,r,tmp);
}
void mergeSort(vector<int> &a) {
    vector<int> tmp(a.size());
    if (!a.empty()) mergeSortRec(a, 0, (int)a.size()-1, tmp);
}

// --- qsort comparator ---
int compare_qsort(const void *pa, const void *pb) {
    int a = *(const int*)pa;
    int b = *(const int*)pb;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

// --- Helper to time a function (returns seconds) ---
template<typename Func>
double timeFunction(Func f) {
    auto t0 = hrclock::now();
    f();
    auto t1 = hrclock::now();
    std::chrono::duration<double> diff = t1 - t0;
    return diff.count();
}

// --- Create distributions ---
enum class Distribution { RANDOM, SORTED, REVERSED, FEW_UNIQUE };

vector<int> generateData(size_t n, Distribution dist, std::mt19937 &rng) {
    vector<int> a(n);
    if (dist == Distribution::RANDOM) {
        std::uniform_int_distribution<int> ud(0, 1000000);
        for (size_t i=0;i<n;++i) a[i] = ud(rng);
    } else if (dist == Distribution::SORTED) {
        for (size_t i=0;i<n;++i) a[i] = (int)i;
    } else if (dist == Distribution::REVERSED) {
        for (size_t i=0;i<n;++i) a[i] = (int)(n - i);
    } else if (dist == Distribution::FEW_UNIQUE) {
        std::uniform_int_distribution<int> ud(0, 10); // only 11 distinct values
        for (size_t i=0;i<n;++i) a[i] = ud(rng);
    }
    return a;
}

// --- Main benchmarking flow ---
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Defaults (can be modified via command-line in future improvements)
    vector<int> sizes = {1000, 5000, 10000, 50000}; // sizes to test
    int runs_per_test = 5; // number of repetitions to average
    bool includeBubble = true; // bubble is very slow for large sizes, toggle as wanted

    // CSV output
    string csvfile = "results.csv";
    ofstream out(csvfile);
    out << "timestamp,algo,size,distribution,avg_time_s,mem_before_kb,mem_after_kb\n";
    out.flush();

    // RNG
    std::random_device rd;
    std::mt19937 rng(rd());

    // Distributions to test
    vector<pair<Distribution,string>> distributions = {
        {Distribution::RANDOM,"random"},
        {Distribution::SORTED,"sorted"},
        {Distribution::REVERSED,"reversed"},
        {Distribution::FEW_UNIQUE,"few_unique"}
    };

    for (int n : sizes) {
        cout << "== Testing size: " << n << " ==\n";
        for (auto &dist_pair : distributions) {
            Distribution dist = dist_pair.first;
            string dist_name = dist_pair.second;
            // Prepare accumulators
            // 1. STL sort (std::sort)
            {
                double total = 0.0;
                long mem_before = -1, mem_after = -1;
                for (int r=0;r<runs_per_test;++r) {
                    auto data = generateData(n, dist, rng);
                    mem_before = getMemoryUsageKB();
                    double t = timeFunction([&]{ std::sort(data.begin(), data.end()); });
                    mem_after = getMemoryUsageKB();
                    total += t;
                }
                double avg = total / runs_per_test;
                out << time(nullptr) << ",stl_sort," << n << "," << dist_name << "," << avg << "," << mem_before << "," << mem_after << "\n";
                out.flush();
                cout << "stl_sort: " << avg << " s (dist="<<dist_name<<")\n";
            }

            // 1b. parallel std::sort (if supported)
#if HAVE_EXECUTION
            {
                double total = 0.0;
                long mem_before = -1, mem_after = -1;
                for (int r=0;r<runs_per_test;++r) {
                    auto data = generateData(n, dist, rng);
                    mem_before = getMemoryUsageKB();
                    double t = timeFunction([&]{
                        // if your implementation supports parallel execution, this will run in parallel
                        std::sort(std::execution::par, data.begin(), data.end());
                    });
                    mem_after = getMemoryUsageKB();
                    total += t;
                }
                double avg = total / runs_per_test;
                out << time(nullptr) << ",stl_sort_parallel," << n << "," << dist_name << "," << avg << "," << mem_before << "," << mem_after << "\n";
                out.flush();
                cout << "stl_sort_parallel: " << avg << " s (dist="<<dist_name<<")\n";
            }
#else
            // If no parallel policy available, note it with same values as stl_sort (or skip)
            cout << "(parallel sort not available in this build)\n";
#endif

            // 2. qsort()
            {
                double total = 0.0;
                long mem_before = -1, mem_after = -1;
                for (int r=0;r<runs_per_test;++r) {
                    auto data = generateData(n, dist, rng);
                    mem_before = getMemoryUsageKB();
                    double t = timeFunction([&]{ qsort(data.data(), data.size(), sizeof(int), compare_qsort); });
                    mem_after = getMemoryUsageKB();
                    total += t;
                }
                double avg = total / runs_per_test;
                out << time(nullptr) << ",qsort," << n << "," << dist_name << "," << avg << "," << mem_before << "," << mem_after << "\n";
                out.flush();
                cout << "qsort: " << avg << " s (dist="<<dist_name<<")\n";
            }

            // 3. merge sort (our implementation)
            {
                double total = 0.0;
                long mem_before = -1, mem_after = -1;
                for (int r=0;r<runs_per_test;++r) {
                    auto data = generateData(n, dist, rng);
                    mem_before = getMemoryUsageKB();
                    double t = timeFunction([&]{ mergeSort(data); });
                    mem_after = getMemoryUsageKB();
                    total += t;
                }
                double avg = total / runs_per_test;
                out << time(nullptr) << ",merge_sort," << n << "," << dist_name << "," << avg << "," << mem_before << "," << mem_after << "\n";
                out.flush();
                cout << "merge_sort: " << avg << " s (dist="<<dist_name<<")\n";
            }

            // 4. bubble sort (be careful: very slow for large n)
            if (includeBubble && n <= 10000) { // safe guard: we only run bubble for ≤10000
                double total = 0.0;
                long mem_before = -1, mem_after = -1;
                for (int r=0;r<runs_per_test;++r) {
                    auto data = generateData(n, dist, rng);
                    mem_before = getMemoryUsageKB();
                    double t = timeFunction([&]{ bubbleSort(data); });
                    mem_after = getMemoryUsageKB();
                    total += t;
                }
                double avg = total / runs_per_test;
                out << time(nullptr) << ",bubble_sort," << n << "," << dist_name << "," << avg << "," << mem_before << "," << mem_after << "\n";
                out.flush();
                cout << "bubble_sort: " << avg << " s (dist="<<dist_name<<")\n";
            } else if (includeBubble) {
                cout << "Skipping bubble sort for n="<<n<<" (too large)\n";
            }
        } // distributions
        cout << "==== done size " << n << " ====\n\n";
    } // sizes

    cout << "Benchmark complete — results saved to " << csvfile << "\n";
    return 0;
}
