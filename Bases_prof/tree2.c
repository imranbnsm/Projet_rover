//
// Created by Imran on 10/11/2024.
//

#include <stdio.h>
#include "tree2.h"
#include "loc.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"

t_position generateRandomPosition(t_map map) {
    t_position randomPos;
    srand(time(NULL));

    do {
        randomPos.x = rand() % map.x_max;
        randomPos.y = rand() % map.y_max;
    } while ( (map.costs[randomPos.x][randomPos.y] !=0 ) && (map.costs[randomPos.x][randomPos.y] < 10000 ) ); // Vérifier les coordonnées

    return randomPos;
}

t_orientation generateRandomOrientation() {
    srand(time(NULL)); //
    return (t_orientation)(rand() % 4);
}

t_tree createTree(t_map map)
{
    t_tree tree;
    t_localisation robot;
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();
    int costo = map.costs[robot.pos.x][robot.pos.y];
    tree.root = createNode(robot, costo, 0);
    return tree;
}


t_tree insertInTree(t_tree tree, t_node *newNode)
{
    if (tree.root == NULL)
    {
        tree.root = newNode;
    }
    return tree;
}


void displayTree(t_node *root, int depth)
{
    if (root == NULL)
        return;

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    displayNode(root);

    for (int i = 0; i < root->num_children; i++)
    {
        displayTree(root->children[i], depth + 1);
    }
}

// recherche d'une feuille de valeur minimale 

/* On crée une fonction auxilaire pour pouvoir faire une fonction recursive, qui prendre en argument le noeud où on se situe, la position de la station,
le noeud avec le coup minimum actuel, et sa valeur. */
t_node *SearchLeafMinAuxiliaire(t_node *node, t_localisation loc, t_node **min_cost_node, int *min_cost) {
    if (node == NULL) return NULL;

    // Vérifie si le noeud actuel se situe à la sation de base et s'il a un coût inférieur au minimum actuel.
    if (node->loc.x == loc.x && node->loc.y == loc.y && node->cost < *min_cost) {
        *min_cost = node->cost;
        *min_cost_node = node;
    }

    // Y a plusieurs noeuds qui ont pour position la station de base, c'est pour ça qu'on continue de parcourir d'autres noeuds pour savoir lequel est le plus efficace.
    for (int i = 0; i < node->num_children; i++) {
        SearchLeafMinAuxiliaire(node->children[i], loc, min_cost_node, min_cost); // On appelle la fonction recursivement pour trouver le noeud avec le coût le plus bas.
    }

    return *min_cost_node;
}

/* On va creer une fonction qui va nous donner la feuille avec la valeur la plus petite, pour faire ça on va prendre en argument l'arbre en question, 
et la position finale où l'on veut être c'est à dire la station de base */
t_node *SearchLeafMin(t_tree tree, t_localisation loc) {
    t_node *min_cost_node = NULL; // Noeud avec le cout minimum.
    int min_cost = 10000;         

    // Appelle de la fonction auxiliaire pour rechercher le noeud feuille avec le coût minimum
    return SearchLeafMinAuxiliaire(tree.root, loc, &min_cost_node, &min_cost);
}



