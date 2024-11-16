//
// Created by Imran on 10/11/2024.
//

#include <stdio.h>
#include "tree.h"
#include "loc.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "moves.h"

t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
t_move movesrobot[9];//appeler nouvelle fonction random moves dans moves.c

//pour le move en string appeler fonction movesAstring dans moves.c

void random_moves() {
    srand(time(NULL));
    int dispo_moves[] = {22, 15, 7, 7, 21, 21, 7};
    for(int i = 0; i<9; i++){
        int indice = rand()%7;
        movesrobot[i] = moves[indice];
        dispo_moves[indice]--;
    }

}

t_position generateRandomPosition(t_map map) {
    t_position randomPos;
    srand(time(NULL));

    do {
        randomPos.x = rand() % map.x_max;
        randomPos.y = rand() % map.y_max;
    } while ( (map.costs[randomPos.y][randomPos.x] == 0 ) && (map.costs[randomPos.y][randomPos.x] > 10000 ) ); // Vérifier les coordonnées

    return randomPos;
}

t_orientation generateRandomOrientation() {
    srand(time(NULL));
    return (t_orientation)(rand() % 4);
}

t_tree createTree(t_map map)
{
    t_tree tree;
    t_localisation robot;
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();
    int cost = map.costs[robot.pos.y][robot.pos.x];
    tree.root = createNode(robot, cost, 0);
    return tree;
}


void insertInTree(t_tree *tree, const int* list_moves, int length_moves, t_map map)
{   t_node *nd=tree->root;
    for (int i=0;i<length_moves-1;i++){
        nd=nd->children[list_moves[i]];    // boucle pour acceder au parent du dernier noeud en fonction des mouvements
        // on va jusqu'à lenght_moves-2, le list_moves[lenght_moves-1] sera attribue à notre nouveau noeud
    }
    t_localisation new_pos = move(nd->loc,movesrobot[list_moves[length_moves-1]]);    // On trouve la nouvelle localisation du dernier noeud en fonction de
    // la localisation du noeud parent et du mouvement qui devra être effectué
    if (isValidLocalisation(new_pos.pos,map.x_max,map.y_max)){
        int cost=map.costs[new_pos.pos.y][new_pos.pos.x];    // Verifier si c'est pas l'inverse pour les pos (d'abord x puis y)
        t_node *nd_child = createNode(new_pos, cost, nd->depth+1);
        addChild(nd, nd_child);
        nd_child->move=movesrobot[list_moves[length_moves-1]];
        for(int j=0;j<length_moves;j++){
            nd_child->move_interdit[j]=list_moves[j]; // Pourquoi on les interdits, on a le droit de reutiliser les mêmes mouvements normalements.
        }
        nd_child->depth=nd->depth+1;
    }
}

void completeTree(t_tree *tree, t_map map) {
     t_node *nd=tree->root;
     for (int l=0;l<9;l++) {
         if (nd->depth == 0) {
             int list_moves[] = {l};
             insertInTree(tree, list_moves, 1, map); // On crée les premiers fils de la racine
         }
         auxiCompleteTree(tree, nd->children[l], map);
     }
}


void auxiCompleteTree(t_tree *tree, t_node *nd, t_map map) {
    if (nd->depth == 5) {  // Limite la profondeur de l'arbre
        return;
    }
    for (int l = 0; l < 9-nd->depth; l++) {  // Parcourir tous les mouvements possible
        printf("%d ",nd->depth);
        int list_moves[nd->depth + 1];
        int j=0;

        // Remplir la liste des mouvements
        for (int k = 0; k < nd->depth-1; k++) {
            list_moves[k] = nd->move_interdit[k];
        }

        list_moves[nd->depth-1] = (int) nd->move;
        list_moves[nd->depth] = l;  // Ajouter le mouvement actuel

        if (movesrobot[l] != nd->move && movesrobot[l] != movesrobot[nd->move_interdit[j]]) {  // Vérifier que le mouvement n'est pas interdit
            printf("c ");
            insertInTree(tree, list_moves, nd->depth + 1, map);
        }
    }

    // Appeler récursivement auxiCompleteTree pour chaque enfant
    for (int i = 0; i < nd->num_children; i++) {
        if (nd->children[i] != NULL) {
            auxiCompleteTree(tree, nd->children[i], map);
        }
    }
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

/* On crée une fonction auxiliaire pour pouvoir faire une fonction recursive, qui prendra en argument le noeud où on se situe, la position de la station,
le noeud avec le coup minimum actuel et sa valeur. */

void SearchLeafMinAuxiliaire(t_node *node, t_node **min_cost_node, int *min_cost) {

    // Vérifie si le noeud actuel a un coût inférieur au minimum actuel.
    
    if (node->num_children == 0 && node->cost < *min_cost) {
        *min_cost = node->cost;
        *min_cost_node = node;
    }

    // Y a plusieurs feuilles qui peuvent avoir un cout plus faible.
    
    for (int i = 0; i < node->num_children; i++) {
        SearchLeafMinAuxiliaire(node->children[i], min_cost_node, min_cost);    // On appelle la fonction recursivement pour trouver le noeud avec le coût le plus bas.
    }

}

/* On va creer une fonction qui va nous donner la feuille avec la valeur la plus petite, pour faire ça on va prendre en argument l'arbre en question, 
et la position finale où l'on veut être c'est à dire la station de base */

t_node *SearchLeafMin(t_tree tree) {
    t_node *min_cost_node = NULL;     // Noeud avec le cout minimum.
    int min_cost = 10000;         

    // Appelle de la fonction auxiliaire pour rechercher le noeud feuille avec le coût minimum
    
   SearchLeafMinAuxiliaire(tree.root, &min_cost_node, &min_cost);
   return min_cost_node;
}


// chemin depuis la racine vers cette feuille.

t_node** CheminRacineFeuille(t_tree tree, t_node* node);


