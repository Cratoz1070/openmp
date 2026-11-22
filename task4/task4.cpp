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

    int n;
    int p;

    cout << "Размер матрицы: ";
    cin >> n;
    cout << "Количество потоков: ";
    cin >> p;

    if (n <= 0 || p <= 0) {
        cerr << "Введите положительные числа" << endl;
        return 1;
    }

    ofstream f("data.txt");
    f << n << endl;

    srand(time(nullptr));
    for (int i = 0; i < n * n; i++) {
        f << rand() % 10000 << ' ';
    }
    f.close();

    int c1 = system(".\\seq.exe data.txt seq.txt");
    int c2 = system((".\\red.exe data.txt red.txt " + to_string(p)).c_str());
    int c3 = system((".\\nored.exe data.txt nored.txt " + to_string(p)).c_str());

    if (c1 != 0 || c2 != 0 || c3 != 0) {
        cerr << "Ошибка запуска программ" << endl;
        return 1;
    }

    int y1, y2, y3;
    double t1, t2, t3;

    read("seq.txt", y1, t1);
    read("red.txt", y2, t2);
    read("nored.txt", y3, t3);

    if (y1 != y2 || y1 != y3) {
        cerr << "Результаты не совпадают" << endl;
        return 1;
    }

    cout << endl;
    cout << "Максимум среди минимумов строк: " << y1 << endl;
    cout << "Последовательно: " << t1 << " с" << endl;
    cout << "С редукцией: " << t2 << " с" << endl;
    cout << "Без редукции: " << t3 << " с" << endl;

    return 0;
}
