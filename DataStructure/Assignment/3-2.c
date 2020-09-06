#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _HEAP_H
#define _HEAP_H

#define MAX_VERTICES 500
#define MAX_HEAP ((MAX_VERTICES-1)*MAX_VERTICES)/2

typedef enum { false, true } bool;
typedef struct{
	double weight;
	short u;
	short v;
} Data;

typedef struct {
	Data data;
	double priority;
} HNode;

typedef struct {
	HNode items[MAX_HEAP + 1];
	int num;
} Heap;


// Make a heap empty.
void InitHeap(Heap *pheap);
// check whether a heap is empty.
bool IsEmpty(Heap *pheap);
// Check whether a heap is full.
bool IsFull(Heap *pheap);

// Get a parent index for a given index.
int GetParent(int idx);
// Get a left child index for a given index.
int GetLChild(int idx);
// Get a right child index for a given index.
int GetRChild(int idx);
// Get a child index with low priority between two child nodes.
int GetLowPrioityChild(Heap* pheap, int idx);

// Insert a node to the heap.
void Insert(Heap *pheap, Data data, double priority);
// Remove the minimum data from the heap.
Data Delete(Heap *pheap);

#endif

double MST(double** arr, int m, int n);
void insert_all_edges(Heap *pheap,double **arr,int m, int n);
double kruskal(int num, Heap *pheap, Data * e,double **arr,int m, int n);
int find(int n);
void merge(int a, int b);
int p[MAX_VERTICES];

int main()
{
    int v;
    int i,j;
    int a,b;
    double mst;

    scanf("%d", &v);
    double ** arr = (double **)malloc(sizeof(double*)*v);
    for(i=0;i<v;i++){
        arr[i] = (double *)malloc(sizeof(double)*v);
    }
    Data * vertex = (Data *)malloc(sizeof(Data)*v);

    for(i=0;i<v;i++){
        scanf("%d %d", &a,&b);
        vertex[i].u = a;
        vertex[i].v = b;
        vertex[i].weight = 0;
    }
    for(i=0;i<v;i++){
        for(j=0;j<v;j++){
            arr[i][j] = sqrt(pow(vertex[i].u - vertex[j].u, 2) + pow(vertex[i].v - vertex[j].v, 2));
        }
    }
    
    mst = MST(arr, v, v);

    printf("%.2f\n", mst);
	return 0;
}
// Minimum Spanning Tree
double MST(double** arr, int m, int n)
{
    Heap h;
    InitHeap(&h);
    Data* edges;
    edges = (Data*)malloc(sizeof(Data)*(m-1));
    double MST;

    MST = kruskal(m,&h, edges,arr,m,n);

    return MST;
}
void insert_all_edges(Heap *pheap,double **arr,int m, int n){
    int i,j;
    Data temp;

    for(i=0;i<m;i++){
        for(j=i+1;j<n;j++){
            if(arr[i][j] != 0){
                temp.weight = arr[i][j];
                temp.u = i;
                temp.v = j;

                Insert(pheap,temp,arr[i][j]);
            }
        }
    }
}
double kruskal(int num, Heap *pheap, Data * e,double **arr,int m, int n){
    int i;
    int edge_accepted = 0;
    int uset,vset;
    Data d;
    double count=0;

    for(i=0;i<MAX_VERTICES;i++){
        p[i] = -1;
    }
    insert_all_edges(pheap,arr,m,n);

    while(edge_accepted < (num-1)){
        if(IsEmpty(pheap)){
            return -1;
        }
        d = Delete(pheap);
        uset = find(d.u);
        vset = find(d.v);
        if( uset != vset){
            count += d.weight;
            e[edge_accepted] = d;
            edge_accepted++;
            merge(uset,vset);
        }
    }
    return count;
}
int find(int n){
    if(p[n] < 0){
        return n;
    }
    p[n] = find(p[n]);
    return p[n];
}
void merge(int a, int b){
    a = find(a);
    b = find(b);
    if(a==b){
        return;
    }
    p[a] += p[b];
    p[b] = a;
}
// Make a heap empty.
void InitHeap(Heap *pheap)
{
	pheap->num = 0;
}

// check whether a heap is empty.
bool IsEmpty(Heap *pheap)
{
	return pheap->num == 0;
}

// Check whether a heap is full.
bool IsFull(Heap *pheap)
{
	return pheap->num == MAX_HEAP;
}

// Get a parent index for a given index.
int GetParent(int idx)
{
	return idx / 2;
}

// Get a left child index for a given index.
int GetLChild(int idx)
{
	return idx * 2;
}

// Get a right child index for a given index.
int GetRChild(int idx)
{
	return idx * 2 + 1;
}

// Get a child index with low priority between two child nodes.
int GetLowPrioityChild(Heap* pheap, int idx)
{
	if (GetLChild(idx) > pheap->num)	// No child nodes exist.
		return 0;
	else if (GetLChild(idx) == pheap->num) // Exist a left child only.
		return GetLChild(idx);
	else // Choose a child node with the lowest priority.
	{
		int left = GetLChild(idx), right = GetRChild(idx);
		if (pheap->items[left].priority < pheap->items[right].priority)
			return left;
		else
			return right;
	}
}

// Insert a node to the heap.
void Insert(Heap *pheap, Data data, double priority)
{
	HNode newNode;
	int idx = pheap->num + 1;
	if (IsFull(pheap))
		exit(1);
	// Compare the new node with its parent.
	while (idx > 1)
	{
		int parent = GetParent(idx);
		if (priority < pheap->items[parent].priority)
		{
			pheap->items[idx] = pheap->items[parent];
			idx = parent;
		}
		else
			break;
	}

	newNode.data = data;
	newNode.priority = priority;

	pheap->items[idx] = newNode;
	pheap->num++;
}

// Remove the minimum data from the heap.
Data Delete(Heap *pheap)
{
	Data min = pheap->items[1].data;
	HNode last = pheap->items[pheap->num];
	int parent = 1, child;

	while (child = GetLowPrioityChild(pheap, parent))
	{
		if (last.priority > pheap->items[child].priority)
		{
			pheap->items[parent] = pheap->items[child];
			parent = child;
		}
		else
			break;
	}

	pheap->items[parent] = last;
	pheap->num--;
	
	return min;
}
