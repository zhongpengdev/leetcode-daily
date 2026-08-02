#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 10
#define INCREMENT 5 // 每次扩容的大小

typedef struct
{
    int *data;
    int length;
    int capacity;
} Seqlist;

// 初始化时分配data指针默认大小的空间
void initSeqList(Seqlist *L)
{
    L->data = (int *)malloc(INIT_SIZE * sizeof(int));

    if (!L->data)
    {
        printf("failed to init");
        exit(1);
    }

    L->length = 0;
    L->capacity = INIT_SIZE;
}

// list扩容，每次扩容固定大小的位置
void expend(Seqlist *L)
{
    // 错误：第一个参数应该是L->data而不是L
    // 错误：第二个参数应该是整体的容量，而不是增加的
    int *newData = (int *)realloc(L->data, (L->capacity + INCREMENT) * sizeof(int));

    if (!newData)
    {
        printf("realloc failed");
        exit(0);
    }

    L->data = newData;
    L->capacity += INCREMENT;
}

// 按位置插入，在第i个位置插入（i从1开始）
int insertAt(Seqlist *L, int pos, int value)
{
    // 检查越界
    if (pos < 1 || pos > L->length + 1)
    {
        printf("插入位置不合法");
        
        // exit(0);是直接杀死进程
        return 0;
    }

    // 判断是否需要扩容
    if (L->length >= L->capacity)
    {
        expend(L);
    }

    // 移动待插入位置及后面全部的元素
    for (int i = L->length; i >= pos; i--)
    {
        L->data[i] = L->data[i - 1];
    }

    // 插入元素
    L->data[pos - 1] = value;
    L->length++;

    return 1;
}

// 尾插法
int pushBack(Seqlist *L, int value)
{
    return insertAt(L, L->length + 1, value);
}

// 按位置删除操作，第pos个位置，从1开始，返回被删除元素
int deleteAt(Seqlist *L, int pos, int *delValue)
{
    // 检查位置是否合法
    if (pos < 1 || pos > L->length)
    {
        printf("it's a illegal pos");
        return 0;
    }

    // 拿到pos位置元素
    // 错误：先解引用
    *delValue = L->data[pos - 1];

    // 将后面所有元素前移一位
    for (int i = pos - 1; i < L->length - 1; i++)
    {
        L->data[i] = L->data[i + 1];
    }

    // length-1
    L->length--;

    return 1;
}

// 按照值删除，（只删除第一个）
int deleteByValue(Seqlist *L, int value)
{
    // 记录第一个匹配的索引
    int index = -1;

    for (int i = 0; i < L->length; i++)
    {
        if (L->data[i] == value)
        {
            // 赋值
            index = i;
            // 错误：需要break
            break;
        }
    }

    if (index == -1)
    {
        return 0;
    }

    for (int i = index; i < L->length - 1; i++)
    {
        L->data[i] = L->data[i + 1];
    }

    // 错误：没更新length
    L->length--;
    return 1;
}

// 按值查找，返回位置，从1开始
int findValue(Seqlist *L, int value) {
    for (int i = 0; i < L->length; i++)
    {
        if(L->data[i] == value) {
            return i + 1;
        }
    }

    // 未找到
    return -1; 
}

// seqList判空
int isEmpty(Seqlist *L) {
    return L->length == 0;
}

// 销毁，释放内存
void destroySequlList(Seqlist *L) {
    if (L->data)
    {
        free(L->data);
        L->data = NULL;
    }

    L->capacity = 0;
    L->length = 0;
}