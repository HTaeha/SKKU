#include "redblacktree.h"
#include <stdio.h>
#include <stdlib.h>

//left rotate
//Change y's left child to x's right child and change x to y's left child.
void rotate_left(rbt_tree *T, rbt_node *x){
    rbt_node* y = x->right;
    x->right = y->left;
    if(y->left != NIL){ //If y has left child, parent of y's left child is x.
        y->left->parent = x;
    }
    y->parent = x->parent; //Change y's parent to x's parent.
    //Change y is child of x's parent.
    if(x->parent == NIL){ //If x is root, change y to root.
        T->root = y;
    }else if(x == x->parent->left){      
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->left = x; //Change y's left child to x.
    x->parent = y; //Change x's parent to y.
}
//right rotate
//Change y's right child to x's left child and change x to y's right child.
void rotate_right(rbt_tree *T, rbt_node *x){
    rbt_node* y = x->left;
    x->left = y->right;
    if(y->right != NIL){ //If y has right child, parent of y's right child is x.
        y->right->parent = x;
    }
    y->parent = x->parent; //Change y's parent to x's parent.
    //Change y is child of x's parent.
    if(x->parent == NIL){ //If x is root, change y to root.
        T->root = y;
    }else if(x == x->parent->right){
        x->parent->right = y;
    }else{
        x->parent->left = y;
    }
    y->right = x; //Change y's right child to x.
    x->parent = y; //Change x's parent to y.
}
//Create and initialize rbt_tree.
rbt_tree *rbt_create(){
    rbt_tree* T = (rbt_tree*)malloc(sizeof(rbt_tree)); //Create rbt_tree.

    T->root = NIL; //Initialize rbt_tree.

    return T; //Return created rbt_tree.
}
//Create and initialize rbt_node and insert rbt_node into rbt_tree.
rbt_node *insert(rbt_tree *T, int key){
    rbt_node* x = (rbt_node*)malloc(sizeof(rbt_node)); //Create rbt_node.

    //Initialize rbt_node.
    x->key = key;
    x->parent = NIL;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;

    insert_rbt(T, x); //Insert rbt_node into rbt_tree.
    return x; //Return created rbt_node.
}
//Insert new node z into tree T.
void insert_rbt(rbt_tree *T, rbt_node *z){
    rbt_node* y = NIL;
    rbt_node* x = T->root;
    //Find z's position.
    while(x != NIL){
        y = x; //y is x's parent.
        if(z->key < x->key){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    z->parent = y; //z's parent is y.
    if(y == NIL){ //If z is the first node in tree T, z is root.
        T->root = z;
    }else if(z->key < y->key){ //z is the left child of y.
        y->left = z;
    }else{ //z is the right child of y.
        y->right = z;
    }
    //z's children are NULL.
    z->left = NIL;
    z->right = NIL;
    //The color of insert node is red, then change in function of insert_rbt_fixup.
    z->color = RED;
    insert_rbt_fixup(T, z);
}
//Reconstitute tree T.
void insert_rbt_fixup(rbt_tree *T, rbt_node *z){
    rbt_node* y;
    //If z is root or the color of z's parent is black, break while loop.
    while(z->parent != NIL && z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right; //y is z's uncle.
            if(y != NIL && y->color == RED){ //y is exist and the color of y is red.
                //Change the color of z's parent and uncle to black.
                z->parent->color = BLACK;
                y->color = BLACK;
                //Change the color of z's grand parent to red.
                z->parent->parent->color = RED;
                //Change z to z's grand parent and go back to the beginning of while loop.
                z = z->parent->parent;
            }else{ //y is null or the color of y is black.
                if(z == z->parent->right){ //z is the right child of z's parent.
                    //Left rotate at z's parent.
                    z = z->parent;
                    rotate_left(T, z);
                }
                //Change the color of z's parent to black.
                //Change the color of z's grand parent to red.
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                //Right rotate at z's grand parent and go back to the begginning of while loop.
                rotate_right(T, z->parent->parent);
            }
        }else{
            y = z->parent->parent->left; //y is z's uncle.
            if(y != NIL && y->color == RED){ //y is exist and the color of y is red.
                //Change the color of z's parent and uncle to black.
                z->parent->color = BLACK;
                y->color = BLACK;
                //Change the color of z's grand parent to red.
                z->parent->parent->color = RED;
                //Change z to z's grand parent and go back to the beginning of while loop.
                z = z->parent->parent;
            }else{ //y is null or the color of y is black.
                if(z == z->parent->left){ //z is the left child of z's parent.
                    //Right rotate at z's parent.
                    z = z->parent;
                    rotate_right(T, z);
                }
                //Change the color of z's paretn to black.
                //Change the color of z's grand parent to red.
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                //Left rotate at z's grand parent and go back to the begginning of while loop.
                rotate_left(T, z->parent->parent);
            }

        }
    }
    //The color of root is always black.
    T->root->color = BLACK;
}
