# C++ 常见库函数速查

---

## 1. `<iostream>` — 输入输出

```cpp
#include <iostream>
using namespace std;

int a;
cin >> a;           // 读入
cout << a << endl;  // 输出并换行
cerr << "error";    // 标准错误输出

// 格式化
cout << fixed << setprecision(2) << 3.14159;  // 3.14
```

---

## 2. `<string>` — 字符串

```cpp
#include <string>

string s = "hello";
// 构造
string s2(5, 'a');     // "aaaaa"
string s3 = to_string(123);  // int → string

// 常用方法
s.length();          // 或 s.size()
s.empty();           // 是否为空
s[0], s.at(0);       // 访问字符
s.substr(pos, len);  // 子串，pos 起始，len 长度
s.find("ll");        // 返回位置，没找到返回 string::npos
s.erase(pos, len);   // 删除
s.insert(pos, "xx"); // 插入
s.append("xx");      // 追加
s += "world";        // 拼接
s.replace(pos, len, "xx"); // 替换
s.c_str();           // 转 const char*
stoi(s);             // string → int
stol(s);             // string → long
stod(s);             // string → double

// 比较
s == s2;  s < s2;  // 按字典序

// 遍历
for (char c : s) { ... }
for (int i = 0; i < s.size(); i++) { ... }
```

---

## 3. `<vector>` — 动态数组

```cpp
#include <vector>

vector<int> v;
vector<int> v2(10);       // 10 个 0
vector<int> v3(10, -1);   // 10 个 -1
vector<int> v4 = {1,2,3}; // 初始化列表

// 增删
v.push_back(x);     // 尾部加
v.pop_back();       // 尾部删
v.insert(v.begin() + i, x);  // 在位置 i 插入
v.erase(v.begin() + i);      // 删除位置 i

// 访问
v[0], v.at(0);
v.front(), v.back();

// 属性
v.size();   v.empty();  v.clear();
v.resize(n);  v.reserve(n);

// 排序
sort(v.begin(), v.end());                // 升序
sort(v.begin(), v.end(), greater<int>()); // 降序

// 查找
auto it = find(v.begin(), v.end(), val); // 返回迭代器，没找到 == v.end()

// 遍历
for (int x : v) { ... }
for (int i = 0; i < v.size(); i++) { ... }
```

---

## 4. `<queue>` — 队列 & 优先队列

```cpp
#include <queue>

// 普通队列（FIFO）
queue<int> q;
q.push(x);      // 入队
q.pop();        // 出队（不返回值）
q.front();      // 队头
q.back();       // 队尾
q.size();       q.empty();

// 优先队列（大顶堆，默认）
priority_queue<int> pq;
pq.push(x);     pq.pop();      pq.top();  // 堆顶（最大值）

// 小顶堆
priority_queue<int, vector<int>, greater<int>> minHeap;

// 结构体优先队列
struct Node { int val, idx; };
struct Cmp {
    bool operator()(const Node& a, const Node& b) {
        return a.val > b.val; // 小顶堆
    }
};
priority_queue<Node, vector<Node>, Cmp> pq2;
```

---

## 5. `<stack>` — 栈

```cpp
#include <stack>

stack<int> st;
st.push(x);      // 压栈
st.pop();        // 弹栈（不返回值）
st.top();        // 栈顶
st.size();       st.empty();
```

---

## 6. `<deque>` — 双端队列

```cpp
#include <deque>

deque<int> dq;
dq.push_back(x);   dq.push_front(x);
dq.pop_back();     dq.pop_front();
dq.front();        dq.back();
dq[0];             dq.at(0);
dq.size();         dq.empty();
```

---

## 7. `<set>` / `<multiset>` — 有序集合

```cpp
#include <set>

set<int> s;             // 有序、不重复
multiset<int> ms;       // 有序、可重复

s.insert(x);
s.erase(x);             // 按值删，或传迭代器
s.find(x);              // 返回迭代器，没找到返回 s.end()
s.count(x);             // 出现次数
s.lower_bound(x);       // >= x 的第一个元素迭代器
s.upper_bound(x);       // > x 的第一个元素迭代器
s.size();  s.empty();

// 遍历（有序）
for (int x : s) { ... }

// 小技巧：用 rbegin() 取最大值
int maxVal = *s.rbegin();
int minVal = *s.begin();
```

---

## 8. `<map>` / `<unordered_map>` — 映射

```cpp
#include <map>
#include <unordered_map>

map<string, int> mp;            // 有序（红黑树）
unordered_map<string, int> ump; // 无序（哈希表，更快）

mp["key"] = 1;
mp.insert({"key", 1});
mp.erase("key");
mp.find("key");          // 返回迭代器，没找到返回 mp.end()
mp.count("key");         // 存在返回 1，否则 0
mp["key"];               // 不存在时自动插入默认值
mp.size();  mp.empty();  mp.clear();

// 遍历
for (auto& [k, v] : mp) { ... }       // C++17 结构化绑定
for (auto& p : mp) { p.first, p.second; } // C++11
```

**选择建议：** leetcode 中 90% 用 `unordered_map`（O(1) 查找）。需要有序遍历时用 `map`。

---

## 9. `<algorithm>` — 常用算法

```cpp
#include <algorithm>

sort(v.begin(), v.end());                    // 排序
reverse(v.begin(), v.end());                 // 反转
int mx = *max_element(v.begin(), v.end());   // 最大值
int mn = *min_element(v.begin(), v.end());   // 最小值
auto it = find(v.begin(), v.end(), val);     // 查找
int cnt = count(v.begin(), v.end(), val);    // 计数
auto it = lower_bound(v.begin(), v.end(), x); // 二分：>= x
auto it = upper_bound(v.begin(), v.end(), x); // 二分：> x
next_permutation(v.begin(), v.end());        // 下一个排列
fill(v.begin(), v.end(), 0);                 // 填充
swap(a, b);                                  // 交换
```

---

## 10. `<numeric>` — 数值运算

```cpp
#include <numeric>

int s = accumulate(v.begin(), v.end(), 0);   // 求和，第三个参数是初始值
int g = gcd(a, b);       // C++17 最大公约数
int l = lcm(a, b);       // C++17 最小公倍数
```

---

## 11. `<utility>` — pair & move

```cpp
#include <utility>

pair<int, int> p = {1, 2};
p.first;   p.second;
auto p2 = make_pair(3, 4);

swap(a, b);
```

---

## 12. `<climits>` / `<limits>` — 数值边界

```cpp
#include <climits>

INT_MAX;    // 2147483647
INT_MIN;    // -2147483648
LONG_LONG_MAX;  LLONG_MAX;  LLONG_MIN;

// C++ 方式
#include <limits>
numeric_limits<int>::max();
numeric_limits<int>::min();
```

---

## 13. `<cmath>` — 数学函数

```cpp
#include <cmath>

abs(x);       // 整数绝对值
fabs(x);      // 浮点绝对值
sqrt(x);      // 平方根
pow(x, y);    // x 的 y 次方
ceil(x);      // 向上取整
floor(x);     // 向下取整
log(x);       // 自然对数
log2(x);      // 以 2 为底
```

---

## 14. `<cstring>` / `<cctype>` — C 风格字符串 & 字符

```cpp
#include <cstring>

strlen(s);    // 长度
strcpy(dst, src);
strcmp(a, b); // 比较，0 相等
memset(arr, 0, sizeof(arr));  // 按字节填充

#include <cctype>

isdigit(c);   isalpha(c);
isupper(c);   islower(c);
toupper(c);   tolower(c);
```

---

## 15. `<bitset>` — 位集合

```cpp
#include <bitset>

bitset<32> bs;        // 32 位，全 0
bitset<32> bs2(42);   // 42 的二进制
bs[0];                // 访问某一位
bs.count();           // 1 的个数
bs.set();             // 全设为 1
bs.reset();           // 全设为 0
bs.flip();            // 取反
bs.to_ulong();        // 转 unsigned long
bs.to_string();       // 转字符串
```

---

## 16. `<tuple>` — 元组（C++17）

```cpp
#include <tuple>

auto t = make_tuple(1, "abc", 3.14);
get<0>(t);   // 1
auto [a, b, c] = t; // 结构化绑定（C++17）
```

---

## 17. LeetCode 常见模板头文件汇总

```cpp
#include <bits/stdc++.h>  // 万能头（竞赛用，非标准）
using namespace std;

// 或标准写法
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <climits>
#include <cmath>
```

---

## 18. 常见坑

| 问题 | 说明 |
|------|------|
| `queue/st stack` 的 `pop()` 不返回值 | 先用 `front()`/`top()` 取值，再 `pop()` |
| `unordered_map` 访问不存在的 key | `mp[key]` 会插入默认值，用 `mp.find()` 或 `mp.count()` 判断存在 |
| `vector` 迭代器失效 | `erase` 后迭代器失效，写 `it = v.erase(it)` |
| `to_string` 精度 | 浮点数默认 6 位小数，用 `ostringstream` 控制 |
| `int` 溢出 | 大数用 `long long`，乘法先转 `long long` |
| `lower_bound` / `upper_bound` | 必须在**有序**容器上调用 |
