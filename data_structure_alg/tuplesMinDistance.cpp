#include<vector>
#include<limits>
#include<cmath>
using namespace std;

int minDistance(vector<int> &S1, vector<int> &S2, vector<int> &S3){
    int i = 0, j = 0, k = 0;
    int len1 = S1.size(), len2 = S2.size(), len3 = S3.size();
    int minDistance = INT_MAX;

    while(i < len1 && j < len2 && k < len3) {
        int a = S1[i], b = S2[j], c = S3[k];
        int tempDinstance = abs(a - b) + abs(b - c) + abs(c - a);

        if( tempDinstance < minDistance) 
            minDistance = tempDinstance;

        int minValue = min(min(a, b), c); //更新最小值
        if(minValue == a)
            i++;
        else if (minValue == b) 
            j++;
        else
            k++;
    }

    return minDistance;
}