#define MAXSIZE 10

typedef struct {
    int data[MAXSIZE];
    int length;
}Sqlist;

//initList
void initList(Sqlist &L) {
    for(int i=0; i<L.length; i++){
        L.data[i] = 0;
    }

    L.length = 0;
}

// 动态内存分配
typedef struct {
    int* data;
    int length;
}DynamicSqlist;

// increaseSize
void increaseSize(DynamicSqlist &L, int len) {
    
}


