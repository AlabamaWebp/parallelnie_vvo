#include <iostream>
#include <iomanip>
#include <vector>
#include <omp.h>

// Вариант 4: интеграл от 0 до 2 (x + 1) dx = 4.
// Численный метод: средние прямоугольники.

double integrate(long long n, int threads, double& elapsed) {
    constexpr double a = 0.0;
    constexpr double b = 2.0;
    const double h = (b - a) / static_cast<double>(n);
    double sum = 0.0;

    const double start = omp_get_wtime();

    #pragma omp parallel num_threads(threads) reduction(+:sum)
    {
        const int tid = omp_get_thread_num();
        const int count = omp_get_num_threads();

        for (long long i = tid; i < n; i += count) {
            const double x = a + (static_cast<double>(i) + 0.5) * h;
            sum += x + 1.0;
        }
    }

    elapsed = omp_get_wtime() - start;
    return sum * h;
}

int main() {
    const std::vector<long long> nodes = {1000000LL, 100000000LL, 1000000000LL};
    const std::vector<int> thread_counts = {1, 2, 4, 8, 12};

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Variant 4: integral of (x + 1) from 0 to 2; exact = 4\n";
    std::cout << "Max OpenMP threads: " << omp_get_max_threads() << "\n\n";

    for (const long long n : nodes) {
        double baseline = 0.0;
        std::cout << "N = " << n << '\n';
        std::cout << "Threads\tIntegral\tTime (s)\tSpeedup\tEfficiency (%)\n";

        for (const int threads : thread_counts) {
            double elapsed = 0.0;
            const double result = integrate(n, threads, elapsed);
            if (threads == 1) baseline = elapsed;

            const double speedup = baseline / elapsed;
            const double efficiency = speedup / threads * 100.0;
            std::cout << threads << '\t' << result << '\t' << elapsed
                      << '\t' << speedup << '\t' << efficiency << '\n';
        }
        std::cout << '\n';
    }
}

