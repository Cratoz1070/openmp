#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    ifstream f(argv[1]);
    int n;
    f >> n;
    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        f >> a[i];
    }

    int mx = a[0];
    int k = 0;
    double start = omp_get_wtime();
    double elapsed;
    volatile int check = 0; // переменная мешает компилятору удалить повторные вычисления.

    do {
        mx = a[0];
        for (int i = 0; i < n; i++) {
            if (a[i] > mx) mx = a[i];
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
