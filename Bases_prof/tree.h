//
// Created by antoi on 08/11/2024.
//

#ifndef PROJET_ROVER_TREE_H
#define PROJET_ROVER_TREE_H
#include "node.h"

struct s_tree {
    node root;
    int height;
};
typedef struct s_tree *tree;

tree create_tree();
void add_node(tree t,int* list_moves, int length, int val);
int search_node(tree t, int val);
void display_tree(tree t);

#endif //PROJET_ROVER_TREE_H
