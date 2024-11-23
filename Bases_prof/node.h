//
// Created by Imran on 10/11/2024.
//

#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H

#include "loc.h"
#include "moves.h"

typedef struct s_node
{
    t_localisation loc;  // Localisation du robot (position + orientation)
    int cost;            // Coût de cette position
    int depth;           // Profondeur dans l'arbre
    struct s_node **children; // Tableau dynamique de fils
    int num_children;    // Nombre de fils
    int* move_interdit; // Liste des mouvements parents du noeud
    t_move move;        // un des 9 types de mouvements
} t_node;

t_node *createNode(t_localisation loc, int cost, int depth);
void addChild(t_node *parent, t_node *child);
void displayNode(t_node *node);
#endif //UNTITLED1_NODE_H
