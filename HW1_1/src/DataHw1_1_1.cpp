#include<iostream>

using namespace std;

int acm(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return acm(m - 1, 1);
    return acm(m - 1, acm(m, n - 1));
}

int main() {
    int m,n;
    cout << "¿é¤JA(m,n):";
    cin >> m >> n;
    cout << acm(m,n) << endl;
    return 0;
}