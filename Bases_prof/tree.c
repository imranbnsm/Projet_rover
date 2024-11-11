//
// Created by antoi on 08/11/2024.
//
#include "tree.h"
#include <stddef.h>
#include <stdio.h>
#include "moves.h"

tree create_tree(){
    tree t;
    t->root= create_node(0,0);
    t->height=5;
    node tmp=t->root;
    for (int i=0;i<5;i++){
        for (int j=0;j<9-i;j++){
            t->root->children[j]=create_node(0,i);
        }

    }
    return t;
}

void add_node(tree t,int* list_moves, int length, int val){
    if (t->root == NULL) {
        // Si l'arbre est vide, créez un nouveau nœud racine avec profondeur 0
        t->root=create_node(val,0);
        t->height=0;
        return;
    }
    node nd=t->root;
    t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
    for (int i=0;i<length;i++){
        nd=nd->children[list_moves[i]];
    }
    nd->value=val;
    nd->move=moves[list_moves[length-1]];
    for (int j=0;j<nd->depth;j++){
        nd->move_interdit[j]=list_moves[j];
    }
}

void display_tree(tree t){
    if (t->root == NULL)
        return;
    printf("aaaaaaaaaaaaaaa");
    for (int i = 0; i < t->height; i++)
    {
        printf("  ");
    }

    printf("Cost: %d, Depth: %d\n", t->root->value, t->root->depth);

    for (int i = 0; i < 9-t->root->depth; i++)
    {
        t->root=t->root->children[i];
        t->height--;
        display_tree(t);
    }
}

//void reset_tree''

