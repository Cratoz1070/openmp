#include <climits>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;
    ifstream f(argv[1]);
    int n, type; f >> n >> type;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) f >> a[i][j];
    int p = 1;
    int result = INT_MIN, k = 0; double start = omp_get_wtime(), elapsed; volatile int check = 0;
    do {
        result = INT_MIN;
        for (int i = 0; i < n; i++) {
            int mn = a[i][0];
            for (int j = 1; j < n; j++) if (a[i][j] < mn) mn = a[i][j];
            if (mn > result) result = mn;
        }
        check = result; k++; elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);
    (void)check;
    ofstream out(argv[2]); out << result << endl << elapsed / k << endl;
    return 0;
}

