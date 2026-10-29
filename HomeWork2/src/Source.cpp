// DS-1 Homework 2 : Polynomial class (dynamic array of nonzero terms)
// 編譯: g++ -std=c++11 -O2 polynomial.cpp -o polynomial
#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>
using namespace std;

long long g_steps = 0;   // 效能量測: 基本運算步數計數器

class Polynomial;  // forward declaration

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
private:
    float coef;  // coefficient
    int exp;     // exponent
};

class Polynomial {
public:
    Polynomial();                              // p(x) = 0
    Polynomial(const Polynomial& rhs);         // copy constructor
    Polynomial& operator=(const Polynomial& rhs);
    ~Polynomial();

    Polynomial Add(Polynomial poly);           // *this + poly
    Polynomial Mult(Polynomial poly);          // *this * poly
    float Eval(float f);                       // p(f)

    friend ostream& operator<<(ostream& out, const Polynomial& p);
    friend istream& operator>>(istream& in, Polynomial& p);

    void NewTerm(float c, int e);              // 新增一項(依指數排序, 相同指數會合併)

private:
    void Append(float c, int e);               // 直接加在尾端 (呼叫者保證 e 比現有指數都小), O(1)
    Term* termArray;   // array of nonzero terms (exp 由大到小排序)
    int capacity;      // size of termArray
    int terms;         // number of nonzero terms
};

// ---------- constructors / destructor ----------
Polynomial::Polynomial() : termArray(new Term[1]), capacity(1), terms(0) {}

Polynomial::Polynomial(const Polynomial& rhs)
    : termArray(new Term[rhs.capacity]), capacity(rhs.capacity), terms(rhs.terms) {
    for (int i = 0; i < terms; i++) termArray[i] = rhs.termArray[i];
}

Polynomial& Polynomial::operator=(const Polynomial& rhs) {
    if (this == &rhs) return *this;
    delete[] termArray;
    capacity = rhs.capacity;
    terms = rhs.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) termArray[i] = rhs.termArray[i];
    return *this;
}

Polynomial::~Polynomial() { delete[] termArray; }

// ---------- helper: insert a term keeping descending exponent order ----------
void Polynomial::NewTerm(float c, int e) {
    if (c == 0) return;
    // 找位置 (由大到小)
    int pos = 0;
    while (pos < terms && termArray[pos].exp > e) { pos++; g_steps++; }

    // 指數相同 -> 係數相加
    if (pos < terms && termArray[pos].exp == e) {
        termArray[pos].coef += c;
        if (termArray[pos].coef == 0) {            // 係數變 0 -> 刪除此項
            for (int i = pos; i < terms - 1; i++) termArray[i] = termArray[i + 1];
            terms--;
        }
        return;
    }
    // 容量不足 -> 倍增
    if (terms == capacity) {
        capacity *= 2;
        Term* tmp = new Term[capacity];
        for (int i = 0; i < terms; i++) tmp[i] = termArray[i];
        delete[] termArray;
        termArray = tmp;
    }
    for (int i = terms; i > pos; i--) { termArray[i] = termArray[i - 1]; g_steps++; }
    termArray[pos].coef = c;
    termArray[pos].exp = e;
    terms++;
}

void Polynomial::Append(float c, int e) {
    if (terms == capacity) {
        capacity *= 2;
        Term* tmp = new Term[capacity];
        for (int i = 0; i < terms; i++) tmp[i] = termArray[i];
        delete[] termArray;
        termArray = tmp;
    }
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;
}

// ---------- Add: 雙指標合併 O(m+n) ----------
Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial c;
    int i = 0, j = 0;
    while (i < terms && j < poly.terms) {
        g_steps++;
        if (termArray[i].exp == poly.termArray[j].exp) {
            float s = termArray[i].coef + poly.termArray[j].coef;
            if (s != 0) c.Append(s, termArray[i].exp);
            i++; j++;
        }
        else if (termArray[i].exp > poly.termArray[j].exp) {
            c.Append(termArray[i].coef, termArray[i].exp); i++;
        }
        else {
            c.Append(poly.termArray[j].coef, poly.termArray[j].exp); j++;
        }
    }
    for (; i < terms; i++)       c.Append(termArray[i].coef, termArray[i].exp);
    for (; j < poly.terms; j++)  c.Append(poly.termArray[j].coef, poly.termArray[j].exp);
    return c;
}

// ---------- Mult: 每一項互乘後累加 O(m*n*k) ----------
Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial c;
    for (int i = 0; i < terms; i++)
        for (int j = 0; j < poly.terms; j++) {
            g_steps++;
            c.NewTerm(termArray[i].coef * poly.termArray[j].coef,
                termArray[i].exp + poly.termArray[j].exp);
        }
    return c;
}

// ---------- Eval: 逐項計算 ----------
float Polynomial::Eval(float f) {
    float result = 0;
    for (int i = 0; i < terms; i++)
        result += termArray[i].coef * std::pow(f, termArray[i].exp);
    return result;
}

// ---------- operator>> : 先輸入項數 n, 再輸入 n 組 (係數 指數) ----------
istream& operator>>(istream& in, Polynomial& p) {
    int n;
    in >> n;
    delete[] p.termArray;
    p.capacity = (n > 0) ? n : 1;
    p.termArray = new Term[p.capacity];
    p.terms = 0;
    for (int i = 0; i < n; i++) {
        float c; int e;
        in >> c >> e;
        if (e < 0) { cerr << "指數必須為非負整數, 已忽略此項\n"; continue; }
        p.NewTerm(c, e);   // 自動排序並合併同次項
    }
    return in;
}

// ---------- operator<< : 例如 3x^2 + 2x - 1 ----------
ostream& operator<<(ostream& out, const Polynomial& p) {
    if (p.terms == 0) return out << "0";
    for (int i = 0; i < p.terms; i++) {
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;
        if (i == 0) { if (c < 0) out << "-"; }
        else { out << (c < 0 ? " - " : " + "); }
        float a = std::fabs(c);
        if (e == 0) out << a;
        else {
            if (a != 1) out << a;
            out << "x";
            if (e != 1) out << "^" << e;
        }
    }
    return out;
}

// ---------- main: 測試 + 效能量測 ----------
int main() {
    Polynomial a, b;
    cout << "輸入多項式 A (格式: 項數 n, 然後 n 組 係數 指數)\n例: 3  3 2  2 1  1 0  代表 3x^2 + 2x + 1\n> ";
    cin >> a;
    cout << "輸入多項式 B\n> ";
    cin >> b;

    cout << "\nA(x) = " << a << "\nB(x) = " << b << "\n";
    cout << "A + B = " << a.Add(b) << "\n";
    cout << "A * B = " << a.Mult(b) << "\n";

    float x;
    cout << "\n輸入 x 值以計算 A(x): ";
    cin >> x;
    cout << "A(" << x << ") = " << a.Eval(x) << "\n";

    // ---- 效能量測: 以基本運算步數計算 (規定標頭檔無計時函式) ----
    cout << "\n===== 效能量測 (步數) =====\n";
    mt19937 gen(12345);
    for (int n : {100, 200, 400, 800, 1600}) {
        Polynomial p, q;
        uniform_int_distribution<int> coefDist(1, 9);
        for (int i = 0; i < n; i++) {          // 每個指數 0..n-1 各一項
            p.NewTerm((float)coefDist(gen), i);
            q.NewTerm((float)coefDist(gen), i);
        }
        g_steps = 0;
        Polynomial s = p.Add(q);
        long long addSteps = g_steps;
        g_steps = 0;
        Polynomial m = p.Mult(q);
        long long multSteps = g_steps;
        cout << "n=" << n << "  Add steps: " << addSteps
            << "  Mult steps: " << multSteps << "\n";
    }
    return 0;
}