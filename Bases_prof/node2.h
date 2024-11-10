//
// Created by Imran on 10/11/2024.
//

#ifndef UNTITLED1_NODE2_H
#define UNTITLED1_NODE2_H

#include "loc.h"

#include "loc.h"

typedef struct s_node
{
    t_localisation loc;  // Localisation du robot
    int cost;            // Coût de cette position
    int depth;           // Profondeur dans l'arbre
    struct s_node **children; // Tableau dynamique de fils
    int num_children;    // Nombre de fils
} t_node;

t_node *createNode(t_localisation loc, int cost, int depth);
void addChild(t_node *parent, t_node *child);
void displayNode(t_node *node);

#endif //UNTITLED1_NODE2_H
