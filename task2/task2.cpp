#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void read(string name, long long& sum, double& t) {
    ifstream f(name);
    f >> sum >> t;

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

    cout << "Размер векторов: ";
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
    for (int i = 0; i < n; i++) {
        f << rand() % 100 << ' ';
    }
    f << endl;

    for (int i = 0; i < n; i++) {
        f << rand() % 100 << ' ';
    }
    f.close();

    int c1 = system(".\\seq.exe data.txt seq.txt");
    int c2 = system((".\\red.exe data.txt red.txt " + to_string(p)).c_str());
    int c3 = system((".\\nored.exe data.txt nored.txt " + to_string(p)).c_str());

    if (c1 != 0 || c2 != 0 || c3 != 0) {
        cerr << "Ошибка запуска программ" << endl;
        return 1;
    }

    long long s1, s2, s3;
    double t1, t2, t3;

    read("seq.txt", s1, t1);
    read("red.txt", s2, t2);
    read("nored.txt", s3, t3);

    if (s1 != s2 || s1 != s3) {
        cerr << "Результаты не совпадают" << endl;
        return 1;
    }

    cout << endl;
    cout << "Скалярное произведение: " << s1 << endl;
    cout << "Последовательно: " << t1 << " с" << endl;
    cout << "С редукцией: " << t2 << " с" << endl;
    cout << "Без редукции: " << t3 << " с" << endl;

    return 0;
}
