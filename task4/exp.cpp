#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void read(string name, int& result, double& t) {
    ifstream f(name);
    f >> result >> t;

    if (!f) {
        cerr << "Ошибка чтения " << name << endl;
        exit(1);
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int sizes[] = {500, 1000, 3000};
    int threads[] = {1, 2, 4, 6, 12, 24};

    ofstream csv("results.csv");
    csv << "size,threads,seq_time,red_time,red_speedup,nored_time,nored_speedup" << endl;

    srand(time(nullptr));

    for (int n : sizes) {
        cout << "Размер матрицы: " << n << endl;

        ofstream data("data.txt");
        data << n << endl;

        for (int i = 0; i < n * n; i++) {
            data << rand() % 10000 << ' ';
        }
        data.close();

        if (system(".\\seq.exe data.txt seq.txt") != 0) {
            return 1;
        }

        int y1;
        double t1;
        read("seq.txt", y1, t1);

        for (int p : threads) {
            cout << "Потоки: " << p << endl;

            string cmd1 = ".\\red.exe data.txt red.txt " + to_string(p);
            string cmd2 = ".\\nored.exe data.txt nored.txt " + to_string(p);

            if (system(cmd1.c_str()) != 0 || system(cmd2.c_str()) != 0) {
                return 1;
            }

            int y2, y3;
            double t2, t3;
            read("red.txt", y2, t2);
            read("nored.txt", y3, t3);

            if (y1 != y2 || y1 != y3) {
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
