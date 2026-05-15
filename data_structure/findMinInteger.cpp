#include <iostream>

int findMinInteger(int array[], int n)
{
    for (int i = 1; i <= n; i++)
    {
        bool isFound = false;

        for (int j = 0; j < n; j++)
        {
            if (array[j] == i)
            {
                isFound = true;
                break;
            }
        }

        if (!isFound)
            return i;
    }

    return n + 1;
}

int main()
{
    int array[5] = {1, 3, 4, 5};
    int min = findMinInteger(array, 4);
    std::cout << min << std::endl;
}