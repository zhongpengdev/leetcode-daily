typedef struct HuffmanNode
{
    HuffmanNode *left;
    HuffmanNode *right;
    int value;
    HuffmanNode(){};
    HuffmanNode(int value): value(value), left(nullptr), right(nullptr){};
    HuffmanNode(HuffmanNode *left, HuffmanNode *right): value(left->value + right->value), left(left), right(right){}; 
};


struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b){
        return a->value > b->value ? true : false;
    }
};