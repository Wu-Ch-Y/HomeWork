#include <iostream>
#include <string>
using namespace std;

void powerset(string s, int i, string cur) {
    if (i == s.length()) {
        cout << "(";
        for (int k = 0; k < cur.length(); k++) {
            cout << cur[k];
            if (k != cur.length() - 1) cout << ",";
        }
        cout << ")" << endl;
        return;
    }

    powerset(s, i + 1, cur);

    powerset(s, i + 1, cur + s[i]);
}

int main() {
    string s = "abc";
    powerset(s, 0, "");
    return 0;
}