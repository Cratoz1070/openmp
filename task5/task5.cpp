#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleCP(CP_UTF8); SetConsoleOutputCP(CP_UTF8);
    int n, p, type;
    cout << "Размер матрицы: "; cin >> n;
    cout << "Потоки: "; cin >> p;
    cout << "Тип матрицы (1 - треугольная, 2 - ленточная): "; cin >> type;
    if (n <= 0 || p <= 0 || (type != 1 && type != 2)) return 1;

    ofstream f("data.txt");
    f << n << ' ' << type << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int value = 0;
            if (type == 1 && j <= i) value = rand() % 10000;
            if (type == 2 && abs(i - j) <= 5) value = rand() % 10000;
            f << value << ' ';
        }
        f << endl;
    }
    f.close();

    string modes[] = {"seq", "static", "dynamic", "guided"};
    for (string mode : modes) {
        string command = ".\\" + mode + ".exe data.txt " + mode + ".txt";
        if (mode != "seq") command += " " + to_string(p);
        if (system(command.c_str()) != 0) return 1;
    }

    cout << endl << "Треугольная матрица" << endl;
    if (type == 2) cout << "Ленточная матрица" << endl;
    for (string mode : modes) {
        ifstream r(mode + ".txt"); int result; double time;
        r >> result >> time;
        cout << mode << ": " << result << ", " << time << " с" << endl;
    }
    return 0;
}
