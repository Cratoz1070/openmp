#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void read(string name, int& mx, double& t) {
    ifstream f(name);
    f >> mx >> t;

    if (!f) {
        cerr << "Ошибка чтения " << name << endl;
        exit(1);
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int sizes[] = {100000, 1000000, 10000000};
    int threads[] = {1, 2, 4, 6, 12, 24};

    ofstream csv("results.csv");
    csv << "size,threads,seq_time,red_time,red_speedup,nored_time,nored_speedup" << endl;

    srand(time(nullptr));

    for (int n : sizes) {
        cout << "Размер: " << n << endl;

        ofstream data("data.txt");
        data << n << endl;

        for (int i = 0; i < n; i++) {
            data << rand() << ' ';
        }
        data.close();

        if (system(".\\seq.exe data.txt seq.txt") != 0) {
            return 1;
        }

        int m1;
        double t1;
        read("seq.txt", m1, t1);

        for (int p : threads) {
            cout << "Потоки: " << p << endl;

            string cmd1 = ".\\red.exe data.txt red.txt " + to_string(p);
            string cmd2 = ".\\nored.exe data.txt nored.txt " + to_string(p);

            if (system(cmd1.c_str()) != 0 || system(cmd2.c_str()) != 0) {
                return 1;
            }

            int m2, m3;
            double t2, t3;
            read("red.txt", m2, t2);
            read("nored.txt", m3, t3);

            if (m1 != m2 || m1 != m3) {
                cerr << "Результаты не совпадают" << endl;
                return 1;
            }

            csv << n << ','
                << p << ','
                << t1 << ','
                << t2 << ','
                << t1 / t2 << ','
                << t3 << ','
                << t1 / t3 << endl;
        }
    }

    cout << "Готово: results.csv" << endl;
    return 0;
}
