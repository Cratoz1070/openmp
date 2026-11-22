#include <cstdlib>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }

    ifstream f(argv[1]);
    int n;
    f >> n;
    vector<int> a(n);
    vector<int> b(n);

    for (int i = 0; i < n; i++) {
        f >> a[i];
    }
    for (int i = 0; i < n; i++) {
        f >> b[i];
    }

    int p = atoi(argv[3]);
    long long sum = 0;
    int k = 0;
    double start = omp_get_wtime();
    double elapsed;
    volatile long long check = 0;

    do {
        sum = 0;

        #pragma omp parallel for num_threads(p) reduction(+:sum)
        for (int i = 0; i < n; i++) {
            sum += a[i] * b[i];
        }

        check = sum;
        k++;
        elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);

    (void)check;
    double t = elapsed / k;
    ofstream out(argv[2]);
    out << sum << endl << t << endl;

    return 0;
}
