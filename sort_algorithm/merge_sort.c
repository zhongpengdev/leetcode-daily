#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int l, int mid, int r)
{
    int n = r - l + 1;
    int *temp = (int *)malloc(n * sizeof(int));

    int i = l;       // 左指针起始索引
    int j = mid + 1; // 右指针起始索引
    int k = 0;       // 数组当前位置索引

    // 比较当前指针值的大小
    while (i <= mid && j <= r)
    {
        if (a[i] <= a[j])
        {
            temp[k] = a[i];
            i++;
        }
        else
        {
            temp[k] = a[j];
            j++;
        }
        k++;
    }

    // 放入左数组剩余元素
    while (i <= mid)
    {
        temp[k++] = a[i++];
    }

    // 右数组的剩余元素
    while (j <= r)
    {
        temp[k++] = a[j++];
    }

    // 覆盖原数组
    for (int i = 0; i < n; i++)
    {
        a[l + i] = temp[i];
    }

    free(temp);
}

void merge_sort(int a[], int l, int r)
{
    if (l == r)
        return; // 终止条件：只有一个元素

    int mid = (l + r) / 2;

    merge_sort(a, l, mid);
    merge_sort(a, mid + 1, r);

    merge(a, l, mid, r); //排序合并子数组
}

int main()
{
    int a[] = {5, 2, 9, 1, 3};
    int n = sizeof(a) / sizeof(a[0]);

    merge_sort(a, 0, n - 1);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    return 0;
}
