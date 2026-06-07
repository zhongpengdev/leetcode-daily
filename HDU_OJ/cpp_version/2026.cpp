#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main()
{
    string line;

    while (getline(cin, line))
    {
        // 处理首字母
        if (!line.empty() && islower(line[0]))
            line[0] = toupper(line[0]);

        // 空格后的字母转大写
        for (size_t i = 1; i + 1 < line.size(); i++)
        {
            if (line[i] == ' ' && islower(line[i + 1]))
                line[i + 1] = toupper(line[i + 1]);
        }

        cout << line << endl;
    }

    return 0;
}
