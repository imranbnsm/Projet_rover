//
// Created by Imran on 10/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "node2.h"

t_node *createNode(t_localisation loc, int cost, int depth)
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->loc = loc;
    node->cost = cost;
    node->depth = depth;
    node->children = NULL;   // Pas d'enfants au début
    node->num_children = 0;   // Pas d'enfants
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
    printf("Position: (%d, %d), Cost: %d, Depth: %d\n", node->loc.pos.x, node->loc.pos.y, node->cost, node->depth);
}
