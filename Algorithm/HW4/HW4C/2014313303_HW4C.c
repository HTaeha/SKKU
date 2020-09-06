#include "header.h"

// Min-heapify the heap at Index.
void minHeapify(MinHeap *M, int Index){
    int smallest = Index;
    int child1; //index of Index node's left child node.
    int child2; //index of Index node's right child node.
    Vertex* Index_v; //Index'th vertex.
    Vertex* child1_v; //child1'th vertex.
    Vertex* child2_v; //child2'th vertex.
    Vertex* smallest_v; //smallest'th vertex.

    child1 = Index*2 + 1;
    child2 = Index*2 + 2;

    Index_v = M->vertexList[M->position[Index]];

    if(child1 < M->remainVertexNumber){ //Check if child1 is a index of the minheap.
        child1_v = M->vertexList[M->position[child1]]; 
        //Store smaller node's index in variable smallest.
        if(Index_v->key <= child1_v->key){
            smallest_v = Index_v;
            smallest = Index;
        }else{
            smallest_v = child1_v;
            smallest = child1;
        }
    }
    if(child2 < M->remainVertexNumber){ //Check if child2 is a index of the minheap.
        child2_v = M->vertexList[M->position[child2]]; 
        //Store smaller one in variable smallest.
        if(child2_v->key < smallest_v->key){
            smallest = child2;
        }
    }
    if(smallest != Index){
        int temp;
        //Exchange position[smallest] and position[Index] in heap.
        temp = M->position[smallest];
        M->position[smallest] = M->position[Index];
        M->position[Index] = temp;
        //minHeapify again.
        minHeapify(M, smallest);
    }
}
// Extract minimum vertex from heap and return it. 
Vertex *extractMin(MinHeap *M){
    Vertex* min;
    int temp;

    min = M->vertexList[M->position[0]]; //Root node of heap has minimum key value.

    //Exchange root node and last node in heap.
    temp = M->position[0];
    M->position[0] = M->position[M->remainVertexNumber-1];
    M->position[M->remainVertexNumber-1] = temp;
    M->remainVertexNumber--;

    minHeapify(M, 0);

    return min;
}
// Decrease key value of vertex V.
void decreaseKey(MinHeap *M, int V, int Key){
    M->vertexList[V]->key = Key; //Change vertex V's key to Key.
}
// Get Vertex from graph and add it to heap.
void addVertexToHeap(MinHeap *M, Graph *G, int V){
    int i;
    for(i=0;i<V;i++){
        //Allocate vertexList.
        M->vertexList[i] = (Vertex*)malloc(sizeof(Vertex));
        //Get vertex from graph and add it to heap.
        M->vertexList[i] = G->vertexList[i];
        //Initialize array position.
        M->position[i] = i;
        M->remainVertexNumber++;
    }
}
// Run Prim's algorithm and return sum of MSP's edge weights.
int primAlgorithm(Graph *G){
    int V = G->totalVertexNumber; //The number of total vertex.
    MinHeap* minHeap = initMinHeap(V); //Initialize minHeap.
    Vertex* u; //vertex u.
    Vertex* v; //vertex v.
    int weight; //Store weight value.
    int i,j,start;
    int isInHeap = 0; //It's a flag some node is in heap or not.
    int result = 0;

    //Add vertex to heap.
    addVertexToHeap(minHeap, G, V);

    //First vertex's key value is 0.
    decreaseKey(minHeap, 0, 0);
    
    while(!isEmptyHeap(minHeap)){ //If heap is empty, stop.
        u = extractMin(minHeap); //vertex u has minimum key value in heap.
        result += u->key;
        for(i=0;i<V;i++){
            weight = u->connectedVertexWeights[i]; //Weight value between u and v.
            if(weight != INT_MAX){ //Find vertex v to connect with vertex u.
                v = G->vertexList[i];
                //Check if v is in the minheap.
                for(j=0;j<minHeap->remainVertexNumber;j++){ 
                    if(minHeap->position[j] == v->name){
                        isInHeap = 1;
                    }
                }
                //If vertex v is in heap and weight is smaller than v's key value, change v's key value to weight.
                if(isInHeap && (weight < v->key)){
                    isInHeap = 0;
                    decreaseKey(minHeap, v->name, weight);
                }
            }
        }
        //Build minHeap.
        start = (minHeap->remainVertexNumber-2)/2;
        for(i=start;i>=0;i--){
            minHeapify(minHeap, i);
        }
    }

    return result;
}
