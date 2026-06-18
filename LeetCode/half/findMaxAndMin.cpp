#include<vector>
#include<algorithm>

struct resultStruct {
    int maxValue;
    int minValue;
};

resultStruct findMAxAndMin(const std::vector<int>& array, int left, int right) {
    resultStruct result; 
    
    if(left == right) {
        result.maxValue = array[left];
        result.minValue = array[left];
        
        return result; //直接结束，终止条件，不能放最后
    }

    if(left == right - 1) {
        result.maxValue = std::max(array[left], array[right]);
        result.minValue = std::min(array[left], array[right]);
            
        return result;
    }

    int mid = left + (right - left) / 2;

    //递归左子树
    resultStruct left_result = findMAxAndMin(array, left, mid);
    //右子树
    resultStruct right_result = findMAxAndMin(array, mid+1, right);

    result.maxValue = std::max(left_result.maxValue, right_result.maxValue);
    result.minValue = std::min(left_result.minValue, right_result.minValue);

    return result;
}