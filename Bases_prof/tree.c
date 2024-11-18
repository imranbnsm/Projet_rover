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
t_move *movesrobot;

//pour le move en string appeler fonction movesAstring dans moves.c

void getMoves(){
    movesrobot = (t_move *)malloc(9 * sizeof(t_move));
    srand(time(NULL));
    int dispo_moves[] = {22, 15, 7, 7, 21, 21, 7};
    for(int i = 0; i<9; i++){
        int indice = rand()%7;
        movesrobot[i] = moves[indice];
        dispo_moves[indice]--;
    }
    return;
} //ne prend pas en compte les probas

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
    getMoves();
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();
    int cost = map.costs[robot.pos.y][robot.pos.x];
    tree.root = createNode(robot, cost, 0);
    completeTree(&tree,map);
    return tree;
}

void insertInTree(t_node *nd, int i_move, t_map map)
{   if (nd != NULL) {
        t_localisation new_pos = move(nd->loc,
                                      movesrobot[i_move]);    // On trouve la nouvelle localisation du dernier noeud en fonction de
        // la localisation du noeud parent et du mouvement qui devra être effectué
        if (isValidLocalisation(new_pos.pos, map.x_max, map.y_max)) {
            int cost = map.costs[new_pos.pos.y][new_pos.pos.x];    // Verifier si c'est pas l'inverse pour les pos (d'abord x puis y)
            t_node *nd_child = createNode(new_pos, cost, nd->depth + 1);
            addChild(nd, nd_child);
            nd_child->move = movesrobot[i_move];
            for (int j = 0; j < nd->depth - 1; j++) {
                nd_child->move_interdit[j] = nd->move_interdit[j]; // Pourquoi on les interdits, on a le droit de reutiliser les mêmes mouvements normalements.
            }
            nd_child->move_interdit[nd->depth - 1] = i_move;
            nd_child->depth = nd->depth + 1;
        }
    }
    return;
}

void completeTree(t_tree *tree, t_map map) {
     if (tree!=NULL && tree->root!=NULL) {
         t_node *nd = tree->root;
         for (int l = 0; l < 9; l++) {
             insertInTree(nd, l, map); // On crée les premiers fils de la racine
         }
         for (int k = 0; k < 9; k++) {
             auxiCompleteTree(nd->children[k], map);
         }
     }
     return;
}


void auxiCompleteTree(t_node *nd, t_map map) {
    if (nd!=NULL && nd->depth>4){
        return;
    } else if (nd!=NULL){
        for (int i=0 ; i<9-nd->depth ; i++){
            int valid_child=1;
            for (int j=0 ; j<nd->depth-1 ; j++){
                if (i == nd->move_interdit[j]) {  // Vérifier que le mouvement n'est pas interdit movesrobots[]
                    valid_child=0;
                    break;
                }
            }
            if (valid_child){
                insertInTree(nd, i, map);
            }
        }

        for (int i = 0; i < nd->num_children; i++) {
            if (nd->children[i]!=NULL) {
                auxiCompleteTree(nd->children[i], map);
            }
        }
        return;
    }
}


void displayTree(t_node *root, int depth) {
    if (root == NULL) {
        return;
    } else {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }

        displayNode(root);

        for (int i = 0; i < root->num_children; i++) {
            displayTree(root->children[i], depth + 1);
        }
    }
    return;
}

// recherche d'une feuille de valeur minimale 

/* On crée une fonction auxiliaire pour pouvoir faire une fonction recursive, qui prendra en argument le noeud où on se situe, la position de la station,
le noeud avec le coup minimum actuel et sa valeur. */

void SearchLeafMinAuxiliaire(t_node *node, t_node **min_cost_node, int *min_cost) {

    if (node == NULL) {
        return;
    }
    
    // Vérifie si le noeud actuel a un coût inférieur au minimum actuel.
    
    if (node->num_children == 0 && node->cost < *min_cost) {
        *min_cost = node->cost;
        *min_cost_node = node;
    }

    // Y a plusieurs feuilles qui peuvent avoir un cout plus faible.
    
    for (int i = 0; i < node->num_children; i++) {
        SearchLeafMinAuxiliaire(node->children[i], min_cost_node, min_cost);    // On appelle la fonction recursivement pour trouver le noeud avec le coût le plus bas.
    }
    return;

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

t_node** CheminRacineFeuille(t_tree tree, t_node* node) {

    if (tree.root == NULL || node == NULL || tree.height <= 0) {
        return NULL;
    }

    t_node** path = (t_node**)malloc((tree.height +1) *  sizeof(t_node*));

    t_node* current = tree.root;
    t_node* minNode = (t_node*)malloc(sizeof(t_node));
    int min, i, j;
    for (i = 0; i < tree.height + 1; i++) {
        min = 10000;
        for (j = 0; j < current->num_children; j++) {
            if (current->children[j]->cost < min) {
                min = current->children[j]->cost;
                minNode = current->children[j];
            }
        }
        path[i] = minNode;
        if (current->num_children == 0) {
            break;
        }
        current = current->children[j];
    }
    return path;
}


