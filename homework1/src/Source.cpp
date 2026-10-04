#include <iostream>
using namespace std;

int ackermann_recursive(int m, int n) {
    // 條件 1: 如果 m = 0，回傳 n + 1
    if (m == 0) {
        return n + 1;
    }
    else if (n == 0) {
        return ackermann_recursive(m - 1, 1);
    }
    else {
        return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
    }
}

int main() {
    int m = 2, n = 1;
    cout << "遞迴結果 A(" << m << ", " << n << ") = "
        << ackermann_recursive(m, n) << endl;
    return 0;
}