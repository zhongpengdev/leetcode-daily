/**
 * 这道题的c版本一直403
 */

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool isLegalIdentifier(const string& s) {
    // 空字符串非法
    if (s.empty()) return false;
    
    // 第一个字符：必须是字母或下划线
    if (!(isalpha(s[0]) || s[0] == '_')) return false;
    
    // 剩余字符：必须是字母、数字或下划线
    for (size_t i = 1; i < s.length(); i++) {
        if (!(isalnum(s[i]) || s[i] == '_')) return false;
    }
    
    return true;
}

int main() {
    int n;
    cin >> n;
    cin.ignore();  // 吸收换行符，重要！
    
    string line;
    for (int i = 0; i < n; i++) {
        getline(cin, line);  // 读取整行（可能包含空格）
        
        if (isLegalIdentifier(line))
            cout << "yes" << endl;
        else
            cout << "no" << endl;
    }
    
    return 0;
}