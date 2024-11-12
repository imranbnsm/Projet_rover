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
t_move movesrobot[9];

void random_moves() {
    srand(time(NULL));
    int dispo_moves[] = {22, 15, 7, 7, 21, 21, 7};
    for(int i = 0; i<9; i++){
        movesrobot[i] = moves[rand()%7];
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
    t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
    for (int i=0;i<length_moves-1;i++){
        nd=nd->children[list_moves[i]];    // boucle pour acceder au parent du dernier noeud en fonction des mouvements
                                           // on va jusqu'à lenght_moves-2, le list_moves[lenght_moves-1] sera attribue à notre nouveau noeud
    }
    t_localisation new_pos = move(nd->loc,moves[list_moves[length_moves-1]]);    // On trouve la nouvelle localisation du dernier noeud en fonction de 
                                                                                 // la localisation du noeud parent et du mouvement qui devra être effectué
    if (isValidLocalisation(new_pos.pos,map.x_max,map.y_max)){
        int cost=map.costs[new_pos.pos.y][new_pos.pos.x];    // Verifier si c'est pas l'inverse pour les pos (d'abord x puis y)
        t_node *nd_child= createNode(new_pos, cost, nd->depth+1);
        addChild(nd, nd_child);
        nd_child->move=moves[list_moves[length_moves-1]];
        for(int j=0;j<length_moves;j++){
            nd_child->move_interdit[j]=list_moves[j]; // Pourquoi on les interdits, on a le droit de reutiliser les mêmes mouvements normalements.
        }
    } else {
        //fin de partie
    }
}
void completeTree (t_tree *tree, t_map map){
    t_node *nd=tree->root;
    t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
    int j=0;
    for (int i=0;i<9;i++){ // 9 mouvements à effectuer
        for (int l=0;l<9-nd->depth;l++) { //  
            if (nd->depth == 0) {
                int list_moves[] = {l};
                insertInTree(tree, list_moves, 1, map); // On crée les premiers fils de la racine
                // en mettant {l}, cela nous permet de faire des fils pour chaque types de mouvements differents.
                // si l = 7 ou 8 ça fait quoi ?
            } else if (moves[nd->move_interdit[j]] != moves[l] && moves[l] != nd->move) { // on verifie que le mouvement du nouveau noeud est different des precedents
                int list_moves[nd->depth + 2]; // ? 
                for (int k = 0; k < nd->depth; k++) {
                    list_moves[k] = nd->move_interdit[k]; // les mouvements de K deviennent des mouvements interdits.
                }
                list_moves[nd->depth] = (int) nd->move;
                list_moves[nd->depth + 1] = l;
                insertInTree(tree, list_moves, nd->depth + 2, map);
            } else {
                continue;
            }
        }
        break;
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

/* On crée une fonction auxilaire pour pouvoir faire une fonction recursive, qui prendra en argument le noeud où on se situe, la position de la station,
le noeud avec le coup minimum actuel et sa valeur. */

t_node *SearchLeafMinAuxiliaire(t_node *node, t_node **min_cost_node, int *min_cost) {
    if (node == NULL) return NULL;

    // Vérifie si le noeud actuel a un coût inférieur au minimum actuel.
    
    if (node->num_children == 0 && node->cost < *min_cost) {
        *min_cost = node->cost;
        *min_cost_node = node;
    }

    // Y a plusieurs feuilles qui peuvent avoir un cout plus faible.
    
    for (int i = 0; i < node->num_children; i++) {
        SearchLeafMinAuxiliaire(node->children[i], min_cost_node, min_cost);    // On appelle la fonction recursivement pour trouver le noeud avec le coût le plus bas.
    }

    return *min_cost_node;
}

/* On va creer une fonction qui va nous donner la feuille avec la valeur la plus petite, pour faire ça on va prendre en argument l'arbre en question, 
et la position finale où l'on veut être c'est à dire la station de base */

t_node *SearchLeafMin(t_tree tree) {
    t_node *min_cost_node = NULL;     // Noeud avec le cout minimum.
    int min_cost = 10000;         

    // Appelle de la fonction auxiliaire pour rechercher le noeud feuille avec le coût minimum
    
    return SearchLeafMinAuxiliaire(tree.root, &min_cost_node, &min_cost);
}


// chemin depuis la racine vers cette feuille.

t_node** CheminRacineFeuille(t_tree tree, t_node* node);


