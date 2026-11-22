#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleCP(CP_UTF8); SetConsoleOutputCP(CP_UTF8);
    int sizes[] = {500, 1000, 2000}, threads[] = {1, 2, 4, 6, 12, 24};
    ofstream csv("results.csv");
    csv << "type,size,threads,seq,static,dynamic,guided" << endl;
    for (int type = 1; type <= 2; type++) for (int n : sizes) {
        ofstream f("data.txt"); f << n << ' ' << type << endl;
        for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) { int x = type == 1 ? (j <= i ? rand() % 10000 : 0) : (abs(i-j) <= 5 ? rand() % 10000 : 0); f << x << ' '; } f << endl; } f.close();
        for (int p : threads) {
            string modes[] = {"seq", "static", "dynamic", "guided"};
            for (string mode : modes) {
                string command = ".\\" + mode + ".exe data.txt " + mode + ".txt";
                if (mode != "seq") command += " " + to_string(p);
                system(command.c_str());
            }
            double t[4]; int result;
            for (int i = 0; i < 4; i++) { ifstream r(modes[i] + ".txt"); r >> result >> t[i]; }
            csv << (type == 1 ? "triangle" : "band") << ',' << n << ',' << p << ',' << t[0] << ',' << t[1] << ',' << t[2] << ',' << t[3] << endl;
        }
    }
    cout << "Готово: results.csv" << endl;
}
