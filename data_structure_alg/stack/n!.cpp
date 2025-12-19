/*
栈实现非递归算法
*/
#include<iostream>
#include<stack>

//递归计算n！O(n)
int func(int n) {
    if (n == 1) return 1;

    return n * func(n-1);
}

//非递归实现阶乘 - 栈：O(n + n) -> O(n)
int funcStack(int n) {
    std::stack<int> s;

    int result = 1;
    
    while(n > 0) {
        s.push(n);
        n--;
    }

    while(!s.empty()) {
        result *= s.top();
        s.pop(); //s.pop()仅移除栈顶元素；
    }

    return result;
}

int main() {
    int n = 5;
    std::cout << func(n) << std::endl;
    std::cout << funcStack(n) << std::endl;
}

