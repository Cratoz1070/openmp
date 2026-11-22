#include <cstdlib>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }

    ifstream f(argv[1]);
    int n;
    double a, b;
    f >> n >> a >> b;

    int p = atoi(argv[3]);
    double h = (b - a) / n;
    double sum = 0;
    double result = 0;
    int k = 0;
    double start = omp_get_wtime();
    double elapsed;
    volatile double check = 0;

    do {
        sum = 0;

        #pragma omp parallel for num_threads(p) reduction(+:sum)
        for (int i = 0; i < n; i++) {
            double x = a + i * h;
            sum += x * x;
        }

        result = sum * h;
        check = result;
        k++;
        elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);

    (void)check;
    double t = elapsed / k;
    ofstream out(argv[2]);
    out.precision(15);
    out << result << endl << t << endl;

    return 0;
}
