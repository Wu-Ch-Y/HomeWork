#include <iostream>
using namespace std;

int ackermann_non_recursive(int m, int n) {
    int stack_arr[100000];
    int top = -1;

    top++;
    stack_arr[top] = m;

    while (top >= 0) {
        m = stack_arr[top];
        top--;

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            top++;
            stack_arr[top] = m - 1;
            n = 1;
        }
        else {
            top++;
            stack_arr[top] = m - 1;

            top++;
            stack_arr[top] = m;

            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m = 2, n = 1;
    cout << "«D»¼°jµ²ªG A(" << m << ", " << n << ") = "
        << ackermann_non_recursive(m, n) << endl;
    return 0;
}