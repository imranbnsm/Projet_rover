//
// Created by Imran on 10/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

t_node *createNode(t_localisation loc, int cost, int depth) // faut peut être intialise le champ move.
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->loc = loc;
    node->cost = cost;
    node->depth = depth;
    node->children=NULL;
    node->children = (t_node**) malloc(depth*sizeof(t_node*));   // Pas d'enfants au début
    node->num_children = 0;   // Pas d'enfants
    node->move_interdit = malloc(depth*sizeof(int));
    return node;
}

void addChild(t_node *parent, t_node *child)
{
    parent->num_children++;
    parent->children = (t_node **)realloc(parent->children, parent->num_children * sizeof(t_node *));
    parent->children[parent->num_children - 1] = child;
}

void displayNode(t_node *node)
{
    printf("Position: (%d, %d), Cost: %d, Depth: %d\n",node->loc.pos.x, node->loc.pos.y, node->cost, node->depth);
}

void freeNode (t_node *node){
    free(node->children);
    free(node->move_interdit);
    free(node);
    return;
}