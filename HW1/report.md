41443110
-
作業一

解題說明
-
本題要求實踐阿克曼函數

**解題策略:**

1.依照阿克曼函式的規則寫出遞迴函式

2.由於阿克曼函數具有超指數的增長特性,使用遞迴的話會很快溢位

3.所以我們使用stack讓程式盡可能的執行

程式實作
-
第一版程式:
```cpp
#include <iostream>
using namespace std;

int ackermann_recursive(int m, int n) {
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
```

第二版程式:
```cpp
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
    cout << "非遞迴結果 A(" << m << ", " << n << ") = " 
         << ackermann_non_recursive(m, n) << endl;
    return 0;
}
```

效能分析
-
* **時間複雜度**: O(A(m, n))
    * m=0: O(1)
    * m=1: O(n)
    * m=2: O(n^2)
    * m=3: O(2^n)
    * m=4: O(2 \uparrow\uparrow n)
* **空間複雜度**: O(A(m, n))

測試與驗證
-
  * 測試一:A(0,1), 預期輸出: 2, 實際輸出:2
  
  * 測試二:A(1,2), 預期輸出: 4, 實際輸出:4
  
  * 測試三:A(2,3), 預期輸出: 9, 實際輸出:9
  
  * 測試四:A(3,4), 預期輸出: 125, 實際輸出:125
  
  * 測試五:A(4,2), 預期輸出: 2^65536-3, 實際輸出:erro
  
  申論及開發報告
  -
  1.遞迴與非遞迴之比較：
  
    第一版簡單易懂,
    然而，由於阿克曼函數的呼叫深度極大，當 m>=4 時，遞迴呼叫會造成"堆疊溢位"。
    第二版雖然較為複雜，需要自行維護堆疊，但能更精準控管記憶體使用情形。

  2.自行實作 Stack 的體會：
  
    在限制不使用標準庫 <stack> 的條件下，透過陣列與指標來實作 Push/Pop 操作，
    讓我們更加深入理解資料結構中"後入先出"的運作原理，以及編譯器在執行遞迴呼叫時於背後維護 Call Stack 的機制。
