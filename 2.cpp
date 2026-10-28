
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <omp.h>

using namespace std;

// Проверка числа на палиндром
bool isPalindrome(int number) {
    if (number < 0) return false;

    int original = number;
    long long reversed = 0;

    do {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    } while (number > 0);

    return original == reversed;
}

int main() {
    int n, threads;

    cout << "Array size: ";
    cin >> n;
    cout << "Number of threads: ";
    cin >> threads;

    if (!cin || n <= 0 || threads <= 0)
        return 1;

    vector<int> a(n);

    mt19937 generator(42);
    uniform_int_distribution<int> dist(0, 999999);

    for (int& x : a)
        x = dist(generator);

    // Последовательный алгоритм
    int sequentialCount = 0;

    double start = omp_get_wtime();

    for (int x : a) {
        if (isPalindrome(x))
            ++sequentialCount;
    }

    double sequentialTime = omp_get_wtime() - start;

    // Параллельный алгоритм
    int parallelCount = 0;
    omp_set_dynamic(0);

    start = omp_get_wtime();

    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for reduction(+:parallelCount)
        for (int i = 0; i < n; ++i) {
            if (isPalindrome(a[i]))
                ++parallelCount;
        }
    }

    double parallelTime = omp_get_wtime() - start;

    cout << "\nSequential count: "
         << sequentialCount << endl;
    cout << "Parallel count:   "
         << parallelCount << endl;

    cout << fixed << setprecision(8);

    cout << "Sequential time: "
         << sequentialTime << " s\n";
    cout << "Parallel time:   "
         << parallelTime << " s\n";

    if (parallelTime > 0) {
        double speedup = sequentialTime / parallelTime;

        cout << "Speedup:    " << speedup << endl;
        cout << "Efficiency: "
             << speedup / threads * 100 << "%\n";
    }

    cout << "Results match: "
         << (sequentialCount == parallelCount
             ? "YES" : "NO") << endl;

    return 0;
}
