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
    for (int i=0;i<5;i++){
        for (int j=0;j<9-i;j++){
            t->root->enfants[j]=create_node(0,i);
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
    t_move moves[]=[(t_move) F_10,(t_move) F_20,(t_move) F_30,(t_move) B_10,(t_move) T_LEFT,(t_move) T_RIGHT,(t_move) U_TURN];
    for (int i=0;i<length;i++){
        nd=nd->enfants[list_moves[i]];
    }
    nd->value=val;
    nd->move=moves[list_moves[length-1]];
    for (int j=0;j<nd->depth;j++){
        nd->move_interdit[j]=list_moves[j];
    }
}

void display_tree(tree t){
    printf("")



}

