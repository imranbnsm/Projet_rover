//
// Created by Imran on 10/11/2024.
//

#ifndef UNTITLED1_TREE2_H
#define UNTITLED1_TREE2_H

#include "node2.h"
#include "map.h"

typedef struct s_tree
{
    t_node *root;  // Racine de l'arbre
    int height;
} t_tree;

t_tree createTree(t_map map);
t_node *searchNode(t_tree tree, t_localisation loc);
void displayTree(t_node *node, int level);
void freeTree(t_tree *tree);
t_tree insertInTree(t_tree tree, t_node *newNode);





#endif //UNTITLED1_TREE2_H
