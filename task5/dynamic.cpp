#include <climits>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) return 1;
    ifstream f(argv[1]);
    int n, type; f >> n >> type;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) f >> a[i][j];
    int p = atoi(argv[3]);
    int result = INT_MIN, k = 0; double start = omp_get_wtime(), elapsed; volatile int check = 0;
    do {
        result = INT_MIN;
        vector<int> local(p, INT_MIN);
        #pragma omp parallel for num_threads(p) schedule(dynamic)
        for (int i = 0; i < n; i++) {
            int mn = a[i][0];
            for (int j = 1; j < n; j++) if (a[i][j] < mn) mn = a[i][j];
            int id = omp_get_thread_num();
            if (mn > local[id]) local[id] = mn;
        }
        for (int i = 0; i < p; i++) if (local[i] > result) result = local[i];
        check = result; k++; elapsed = omp_get_wtime() - start;
    } while (elapsed < 0.05);
    (void)check;
    ofstream out(argv[2]); out << result << endl << elapsed / k << endl;
    return 0;
}

