#include <climits>
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
    vector<vector<int>> a(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            f >> a[i][j];
        }
    }

    int result = INT_MIN;
    int k = 0;
    double start = omp_get_wtime();
    double elapsed;
    volatile int check = 0;

    do {
        result = INT_MIN;

        for (int i = 0; i < n; i++) {
            int mn = a[i][0];

            for (int j = 1; j < n; j++) {
                if (a[i][j] < mn) mn = a[i][j];
            }

            if (mn > result) result = mn;
        }

        check = result;
        k++;
        elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);

    (void)check;
    double t = elapsed / k;
    ofstream out(argv[2]);
    out << result << endl << t << endl;

    return 0;
}
