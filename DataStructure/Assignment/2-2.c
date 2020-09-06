#include <stdio.h>
#include <stdlib.h>

typedef int BData;

typedef struct _bTreeNode
{
    BData item;
    struct _bTreeNode * left_child;
    struct _bTreeNode * right_child;
} BTreeNode;

BTreeNode * CreateNode(BData item);
int CountDepth(BData* arrays, int n);
void ClearTree(BTreeNode* root);

int main(){
    int n;
    int i;
    int depth;
    BTreeNode * r;

    scanf("%d", &n);

    BData *arr = malloc(sizeof(BData)*n);

    for(i=0;i<n;i++){
        scanf("%d", &arr[i]);
    }
    depth = CountDepth(arr, n);
    printf("%d\n", depth);

    return 0;
}
// Build a tree from an array list and count depth.
int CountDepth(BData* arr, int n)
{
    int i;
    BTreeNode * node = NULL;
    BTreeNode * temp = NULL;
    BTreeNode * root = NULL;
    int depth = 0;
    int max = 0;

    for(i=0;i<n;i++){
        node = CreateNode(arr[i]);
        if(i==0){
            root = node;
            continue;
        }
        temp = root;
        depth = 0;
        while(1){
            depth++;
            if(node->item < temp->item){
                if(temp->left_child != NULL){
                    temp = temp->left_child;
                }else{
                    temp->left_child = node;
                    break;
                }
            }else{
                if(temp->right_child != NULL){
                    temp = temp->right_child;
                }else{
                    temp->right_child = node;
                    break;
                }
            }
        }
        if(max < depth){
            max = depth;
        }
    }
    ClearTree(root);
    return max;
}

// Clear a tree.
void ClearTree(BTreeNode* root)
{
    if (root != NULL)
    {
        ClearTree(root->left_child);
        ClearTree(root->right_child);
        free(root);
    }
}
// Create a new node.
BTreeNode * CreateNode(BData item)
{
    BTreeNode * node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->item = item;
    node->left_child = NULL;
    node->right_child = NULL;

    return node;
}

