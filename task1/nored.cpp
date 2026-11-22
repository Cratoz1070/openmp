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

    for (int i = 0; i < n; i++) {
        f >> a[i];
    }

    int p = atoi(argv[3]);
    int mx = a[0];
    int k = 0;
    double start = omp_get_wtime();
    double elapsed;
    volatile int check = 0;

    do {
        mx = a[0];

        #pragma omp parallel num_threads(p) shared(mx)
        {
            int local = a[0];

            #pragma omp for nowait
            for (int i = 0; i < n; i++) {
                if (a[i] > local) local = a[i];
            }

            #pragma omp critical
            {
                if (local > mx) mx = local;
            }
        }

        check = mx;
        k++;
        elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);

    (void)check;
    double t = elapsed / k;
    ofstream out(argv[2]);
    out << mx << endl << t << endl;

    return 0;
}
