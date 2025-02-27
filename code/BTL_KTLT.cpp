#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#ifdef _WIN32
#include <windows.h> // Dùng Sleep trên Windows
#else
#include <unistd.h>  // Dùng usleep trên Linux/macOS
#endif
using namespace std;

#define f first
#define s second
#define pb push_back
#define mp make_pair
typedef vector<pair<int, int>> visited;
typedef pair<int, int> p;

const int dx1[4] = {0, 0, -1, 1};  // Trên, dưới, trái, phải
const int dx2[4] = {-1, 1, 0, 0};

ofstream out;
ifstream in;

struct Maze {
    int x, y;
    int** V;
    bool** check;
};

struct Robot {
    int s = 0;          // Tổng điểm
    p start;            // Vị trí bắt đầu
    vector<p> trace;    // Lộ trình
};

void input(Maze &mz);
void output(Maze mz, Robot r1, Robot r2);
void reset(Maze &mz);
void moveRobot(Maze mz, Robot &r, p now);
void move2Robot(Maze mz, Robot r1, Robot r2);
void visualize(Maze mz, Robot r);
void sameStep(Robot r1, Robot r2);

int main() {
    out.open("Output.txt");
    Maze mz;
    Robot r1, r2;

    input(mz);
    output(mz, r1, r2);
    out.close();
    return 0;
}

void input(Maze &mz) {
    in.open("Input.txt");
    if (!in) {
        cout << "Khong the mo file Input.txt!" << endl;
        exit(1);
    }
    in >> mz.x >> mz.y;

    mz.V = new int*[mz.x];
    mz.check = new bool*[mz.x];
    for (int i = 0; i < mz.x; i++) {
        mz.V[i] = new int[mz.y];
        mz.check[i] = new bool[mz.y];
    }

    vector<int> values;
    for (int i = 0; i < mz.x; i++) {
        for (int j = 0; j < mz.y; j++) {
            in >> mz.V[i][j];
            mz.check[i][j] = true;
            if (find(values.begin(), values.end(), mz.V[i][j]) != values.end()) {
                cout << "Loi: Gia tri trung lap trong mang!" << endl;
                exit(1);
            }
            values.pb(mz.V[i][j]);
        }
    }
    in.close();
}

void output(Maze mz, Robot r1, Robot r2) {
    cout << "Vi tri bat dau cua r1 (x,y): ";
    cin >> r1.start.f >> r1.start.s;
    moveRobot(mz, r1, r1.start);

    out << "Do dai lo trinh r1: " << r1.trace.size() << endl;
    for (auto t : r1.trace)
        out << mz.V[t.f][t.s] << "\t";

    cout << "Duong di cua r1: " << endl;
    visualize(mz, r1);
#ifdef _WIN32
    Sleep(5000); // 5000 ms = 5 giây
#else
    usleep(5000000); // 5000000 μs = 5 giây
#endif

    reset(mz);
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout << "Vi tri bat dau cua r2 (x,y): ";
    cin >> r2.start.f >> r2.start.s;
    moveRobot(mz, r2, r2.start);

    out << "\nDo dai lo trinh r2: " << r2.trace.size() << endl;
    for (auto t : r2.trace)
        out << mz.V[t.f][t.s] << "\t";

    cout << "Duong di cua r2: " << endl;
    visualize(mz, r2);
#ifdef _WIN32
    Sleep(5000);
#else
    usleep(5000000);
#endif

    sameStep(r1, r2);
    move2Robot(mz, r1, r2);
}

void reset(Maze &mz) {
    for (int i = 0; i < mz.x; i++)
        for (int j = 0; j < mz.y; j++)
            mz.check[i][j] = true;
}

void visualize(Maze mz, Robot r) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    string scale = string(mz.y * 4 + 1, '-');
    cout << scale << endl;

    for (int i = 0; i < mz.x; i++) {
        cout << "|";
        for (int j = 0; j < mz.y; j++) {
            cout << " " << mz.V[i][j] << " |";
        }
        cout << endl << scale << endl;
    }

    cout << "Lo trinh: ";
    for (auto t : r.trace) {
        cout << "(" << t.f << "," << t.s << ") ";
#ifdef _WIN32
        Sleep(1000); // 1 giây
#else
        usleep(1000000); // 1 giây
#endif
    }
    cout << "\nTong diem: " << r.s << endl;
}

void moveRobot(Maze mz, Robot &r, p now) {
    r.trace.pb(now);
    r.s += mz.V[now.f][now.s];
    mz.check[now.f][now.s] = false;

    int mx = -1, ind = -1, a, b;
    for (int i = 0; i < 4; i++) {
        a = now.f + dx1[i];
        b = now.s + dx2[i];
        if (a >= 0 && b >= 0 && a < mz.x && b < mz.y && mz.check[a][b]) {
            if (mz.V[a][b] > mx) {
                mx = mz.V[a][b];
                ind = i;
            }
        }
    }
    if (ind != -1) {
        a = now.f + dx1[ind];
        b = now.s + dx2[ind];
        moveRobot(mz, r, mp(a, b));
    }
}

void move2Robot(Maze mz, Robot r1, Robot r2) {
    reset(mz);
    r1.s = 0; r1.trace.clear();
    r2.s = 0; r2.trace.clear();

    moveRobot(mz, r1, r1.start);
    reset(mz);
    moveRobot(mz, r2, r2.start);

    out << "\n\nTong diem cua robot 1: " << r1.s << endl;
    out << "Tong diem cua robot 2: " << r2.s << endl;

    if (r1.s > r2.s)
        out << "Robot 1 thang";
    else if (r1.s < r2.s)
        out << "Robot 2 thang";
    else
        out << "Hoa nhau";
}

void sameStep(Robot r1, Robot r2) {
    vector<p> v1 = r1.trace;
    vector<p> v2 = r2.trace;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    vector<p> res;
    int i = 0, j = 0;
    while (i < v1.size() && j < v2.size()) {
        if (v1[i] == v2[j]) {
            res.pb(v1[i]);
            i++; j++;
        } else if (v1[i] > v2[j]) j++;
        else i++;
    }

    out << "\nNhung vi tri trung nhau: ";
    for (auto t : res)
        out << "(" << t.f << "," << t.s << ") ";
    out << endl;
}