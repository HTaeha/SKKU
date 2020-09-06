#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_SIZE 200

void Print_Tree(int * tree_arr, int tree_index);
int Parent(int node, int X);
void Make_Child_Index(int * child_arr, int X, int node_index);
void Min_Heapify(int * tree_arr, int tree_index, int X, int * child_arr, int node_index);
void Build_Min_Heap(int * tree_arr, int tree_index, int X, int * child_arr);
void HeapSort(int * tree_arr, int tree_index, int X, int * child_arr);
int main(){
    int X;   //Determine X-nary tree.
    int tree[MAX_STR_SIZE];  //Store input numbers. It's a tree.
    char input_c;  //Store character type input.
    int input_i;  //Store integer type input.
    int index=0;  //An index of tree array.
    int flag = 0;  //If an input is '\n', stop to get inputs.
    int * child;  //Store child node index.
    int i;

    scanf("%d",&X);
    while((input_c = getchar()) != '\n'){} //Eliminate ' ', '\n'.

    while(!flag){  //Get inputs.
        input_c = getchar();
        input_i = 0;
        if(input_c == ' '){
            continue;
        }else if(input_c == '\n'){
            break;
        }
        while(1){
            input_i += (input_c - '0');
            input_c = getchar();
            if(input_c == ' '){
                break;
            }else if(input_c == '\n'){
                flag = 1;
                break;
            }else{
                input_i *= 10;
            }
        }
        tree[index++] = input_i;
    }
    index--;  //tree array's last index.

    child = (int*)malloc(sizeof(int)*X); //Store child node in X-nary tree.

    HeapSort(tree, index, X, child);

    for(i=0;i<index;i++){ //Print sorted numbers.
        printf("%d ", tree[i]);
    }
    printf("%d\n",tree[index]);

    free(child);
    child = NULL;  //To avoid dangling reference.

    return 0;
}
void Print_Tree(int * tree_arr, int tree_length){ //Print all tree node values.
    int i;

    for(i=0;i<=tree_length;i++){
        printf("%d ",tree_arr[i]);
    }
    printf("\n");
}
int Parent(int node, int X){ //Find parent node index.
    return (node-1)/X;
}
void Make_Child_Index(int * child_arr, int X, int node_index){ //Store child node index in array.
    int i;

    for(i=0;i<X;i++){
        child_arr[i] = node_index*X + i+1;
    }
}
void Min_Heapify(int * tree_arr, int heap_size, int X, int * child_arr, int node_index){
    int smallest;
    int temp;
    int i;

    Make_Child_Index(child_arr, X, node_index); //Store child nodes index in child_arr array.
    if(child_arr[0] <= heap_size && tree_arr[child_arr[0]] < tree_arr[node_index]){ //Find smaller value among first child node and current node.
        smallest = child_arr[0];
    }else{
        smallest = node_index;
    }

    for(i=1;i<X;i++){ //Find smallest value among child nodes and current node.
        if(child_arr[i] <= heap_size && tree_arr[child_arr[i]] < tree_arr[smallest]){
            smallest = child_arr[i];
        }
    }

    if(smallest != node_index){ //Current node doesn't have smallest value.
        temp = tree_arr[node_index]; //Change smallest node and current node.
        tree_arr[node_index] = tree_arr[smallest];
        tree_arr[smallest] = temp;
        Min_Heapify(tree_arr, heap_size, X, child_arr, smallest);
    }
}
void Build_Min_Heap(int * tree_arr, int tree_length, int X, int * child_arr){ //Build minimum heap.
    int i;
    int start = Parent(tree_length, X); //Start Min_Heapify at last leaf node's parent node.

    for(i=start;i>=0;i--){
        Min_Heapify(tree_arr, tree_length, X, child_arr, i);
    }
}
void HeapSort(int * tree_arr, int length, int X, int * child_arr){
    int i;
    int temp;
    int heap_size = length;

    Build_Min_Heap(tree_arr, length, X, child_arr);

    for(i=length;i>0;i--){
        Print_Tree(tree_arr, length);
        temp = tree_arr[0]; //Change root node and last leaf node.
        tree_arr[0] = tree_arr[i];
        tree_arr[i] = temp;
        heap_size--;  //Heap size minus 1.
        Min_Heapify(tree_arr, heap_size, X, child_arr, 0);
    }
}
