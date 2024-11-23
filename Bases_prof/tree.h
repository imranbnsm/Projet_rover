//
// Created by Imran on 10/11/2024.
//

#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include "node.h"
#include "map.h"
#include "moves.h"
static char _orientation[8][8] = {"Nord","Est","Sud","Ouest"};

typedef struct s_tree
{
    t_node *root;  // Racine de l'arbre
    int height;
} t_tree;

t_tree createTree(t_map map,int,int);
t_node *searchNode(t_tree tree, t_localisation loc);
void insertInTree(t_node *nd, int move, t_map map,int);
void completeTree (t_tree *, t_map map, int);
void auxiCompleteTree(t_node *node,t_map map,int);
void displayTree(t_node *node, int level);
void freeTree(t_node *root);
char* getOrientation (int ori);
void SearchLeafMinAuxiliaire(t_node *node, t_node **min_cost_node, int *min_cost);
t_node *SearchLeafMin(t_tree tree);
void getMoves(int);
t_node** CheminRacineFeuille(t_tree tree);
void play(t_map);



#endif //UNTITLED1_TREE_H
