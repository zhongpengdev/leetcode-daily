#include <vector>
#include <string>
#include<iostream>


// 空的vector
std::vector<int> vec1;

// 指定大小
std::vector<int> vec2(10);

// size=10, value=100
std::vector<int> vec3(10, 100);

std::vector<std::string> fruits = {
    "Apple", "Banana", "Cherry"};

// vector拷贝
std::vector<std::string> vectorCopyFruit(fruits);

//---------------

int main()
{
    // insert into tail
    vec1.push_back(5);

    //删除尾部元素
    vec1.pop_back();

    //任意位置插入
    vec1.insert(vec1.begin()+1, 99);

    std::cout << vec1[1] << std::endl;

    //.size();
    //.capacity(); 还能容纳
    //.empty()

    //查找
    int value1 = vec1[1]; //不进行边界检查，超出范围undefined Behavior

    int value2 = vec1.at(2); //边界检查，out_of_range

    //首尾元素
    //vec1.front()
    //vec1.back()
    
    //返回指针
    int* ptr = vec1.data();

    //------------遍历
    
    //最简洁安全使用for循环
    for(const std::string& fruit : fruits) {
        std::cout << fruit << std::endl;
    }

    //传统for循环
    //迭代器
}
