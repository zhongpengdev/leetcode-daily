/**
 * Data Structure of Linked List.
 */

#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int data;
    struct LNode *next;
} LNode, *LinkList;

// 初始化，返回头节点
LinkList initLinkList()
{
    LinkList L = (LinkList)malloc(sizeof(LinkList));

    if(!L) {
        printf("init LinkList failed");
        exit(0);
    }

    L->next = NULL;
    return L;
}

// 插入元素，头插法，逆序建表
void insertHead(LinkList L, int value) {
    // 初始化一个LNode节点
    LNode *node = (LNode*)malloc(sizeof(LNode));

    // 初始化节点
    node->data = value;
    node->next = L->next;

    L->next = node;
}

// 尾插法，需要O（n）的遍历，设置尾指针实现O(1)
void insertTail(LinkList L, int value) {
    // 找到最后一个节点
    LNode *p = L;
    while (p->next != NULL)
    {
        p = p->next;
    }

    // 分配新LNode节点
    LNode *node = (LNode*)malloc(sizeof(LNode));
    node->data = value;
    node->next = NULL;

    p->next = NULL;
}

// 按照位置插入, 从1开始
int insertAt(LinkList L, int pos, int value) {
    if (pos < 1)
    {
        return 0;     
    }
        
    LNode *p = L;

    // 找到第i-1个前驱节点
    int i = 0;
    while (p != NULL && i < pos - 1)
    {
        p = p->next;
        i++;
    }

    // p是NULL,超出范围
    if (p == NULL)
    {
        return 0;
    }

    // 分配节点
    LNode *s = (LNode*)malloc(sizeof(LNode));
    s->data = value;
    
    s->next = p->next;
    p->next = s;
    return 1;
}

// 按照位置删除，位置从1开始，返回节点的值



// 按照值删除，删除第一个位置
