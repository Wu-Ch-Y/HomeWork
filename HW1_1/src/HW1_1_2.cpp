#include <iostream>
using namespace std;

int st[100000];
int top = 0;

int ack2(int m, int n) {
    top = 0;
    st[top] = m;
    top++;

    while (top > 0) {
        top--;
        m = st[top];

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            n = 1;
            st[top] = m - 1; 
            top++;
        }
        else {
            st[top] = m - 1;
            top++;
            st[top] = m;
            top++;
            n = n - 1;
        }
    }
    return n;
}

int main() {
    cout << ack2(2, 3) << endl;   // µª®×¬O 9
    return 0;
}