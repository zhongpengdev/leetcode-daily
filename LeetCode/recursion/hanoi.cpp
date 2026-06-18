#include<iostream>
#include<string>

void hanoi(int n, const std::string& source, const std::string& auxiliary, const std::string& destination) {
    //if(n==1) then move(A, C)
    if (n == 1) {
        std::cout << "将圆盘1从" << source << "移动到" <<destination <<std::endl;
        return;
     }

     //hanoi(n-1, A, C, B)
     hanoi(n-1, source, destination, auxiliary);

     //move(A,C)
     std::cout << "将圆盘" << n << "从" << source << "移动到" << destination <<std::endl;
     
     //hanoi(n-1, B, A, C)
     hanoi(n-1, auxiliary, source, destination);
}

int main() {
    int num = 3;
    hanoi(num, "A", "B", "C");
    return 0;
}