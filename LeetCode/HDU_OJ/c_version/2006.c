#include <stdio.h>
#include<stdbool.h>



// return number % 2 != 0
bool isOdd(int numer) {
	if(numer % 2 == 0) return false;
	return true;
}



int main()
{
	int n;
	while (scanf("%d", &n) != EOF)
	{
		int result = 1;
		int number;
		for (int i = 0; i < n; i++)
		{
			scanf("%d", &number);
			if(isOdd(number)) {
				result *= number;
			}else {
				continue;
			}
		}
		printf("%d\n", result);
	}

	return 0;
}