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


void getMoves(int is_on_erg) {
    // Allouer de la mémoire pour les mouvements
    movesrobot = (t_move *)malloc(9 * sizeof(t_move));
    if (movesrobot == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour movesrobot.\n");
        exit(EXIT_FAILURE);
    }

    // Probabilités initiales pour chaque type de mouvement
    int nbmoves[] = {22, 15, 7, 7, 21, 21, 7}; // Total 100
    int total_moves = 100; // Total des mouvements

    for (int i = 0; i < 9; i++) {
        int r = rand() % total_moves; // Générer un nombre aléatoire
        int type = 0;

        // Trouver le type de mouvement basé sur le nombre aléatoire
        while (r >= nbmoves[type]) {
            r -= nbmoves[type];
            type++;
        }

        // Décrémenter le nombre de mouvements disponibles pour ce type
        nbmoves[type]--;
        total_moves--;

        // Assigner le type de mouvement à movesrobot
        movesrobot[i] = moves[type]; // Utiliser le tableau de mouvements
    }

    // Adapter les mouvements si le robot est sur un erg
    if (is_on_erg) {
        for (int i = 0; i < 9; i++) {
            switch (movesrobot[i]) {
                case F_10:
                    movesrobot[i] = NONE; // Ne fait rien
                    break;
                case F_20:
                    movesrobot[i] = F_10; // N'avance que de 10m
                    break;
                case F_30:
                    movesrobot[i] = F_20; // N'avance que de 20m
                    break;
                case B_10:
                    movesrobot[i] = NONE; // Ne fait rien
                    break;
                case U_TURN:
                    movesrobot[i] = T_RIGHT; // Ne pas tourner
                    break;
                default:
                    break;
            }
        }
    }
}




t_position generateRandomPosition(t_map map) {
    t_position randomPos;

    do {
        randomPos.x = rand() % map.x_max;
        randomPos.y = rand() % map.y_max;
    } while ( (map.costs[randomPos.y][randomPos.x] == 0 ) || (map.costs[randomPos.y][randomPos.x] > 10000 ) ); // Vérifier les coordonnées

    return randomPos;
}

t_orientation generateRandomOrientation() {
    return (t_orientation)(rand() % 4);
}



t_tree createTree(t_map map, int available_moves) {

    t_tree tree;
    t_localisation robot;
    tree.height = available_moves;
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();
    int cost = map.costs[robot.pos.y][robot.pos.x];
    tree.root = createNode(robot, cost, 0);

    int is_on_erg = (map.costs[robot.pos.y][robot.pos.x] == 2);

    getMoves(is_on_erg); // Passer l'état de la case erg

    printf("Robot initialise a la position (%d, %d) avec orientation %s.\n", robot.pos.x, robot.pos.y, getOrientation((robot.ori)));

    completeTree(&tree, map, available_moves);

    return tree;
}


void insertInTree(t_node *nd, int i_move, t_map map, int available_moves) {
    if (nd != NULL) {
        // Vérifiez si l'indice de mouvement est valide
        if (nd->depth >= available_moves) {
            return;
        }

        t_localisation new_pos = move(nd->loc, movesrobot[i_move]);

        if (isValidLocalisation(new_pos.pos, map.x_max, map.y_max) && nd->cost < 10000) {
            int cost = map.costs[new_pos.pos.y][new_pos.pos.x];
            t_node *nd_child = createNode(new_pos, cost, nd->depth + 1);

            if (nd_child == NULL) {
                printf("Error: Failed to create child node.\n");
                return;
            }

            addChild(nd, nd_child);
            nd_child->move = movesrobot[i_move];

            if (nd->move_interdit != NULL) {
                for (int j = 0; j < nd->depth - 1; j++) {
                    if (nd->depth >= 1) {
                        nd_child->move_interdit[j] = nd->move_interdit[j];
                    }
                }
            }

            nd_child->move_interdit[nd->depth - 1] = i_move;
            nd_child->depth = nd->depth + 1;
        }
    }
}


void completeTree(t_tree *tree, t_map map, int available_moves) {

    if (tree != NULL && tree->root != NULL) {

        t_node *nd = tree->root;

        for (int l = 0; l < 9; l++) {

            insertInTree(nd, l, map, available_moves);

        }

        for (int k = 0; k < nd->num_children; k++) {

            auxiCompleteTree(nd->children[k], map, available_moves);

        }
    }
}


void auxiCompleteTree(t_node *nd, t_map map, int available_moves) {

    if (nd==NULL || nd->depth<1 || nd->depth==5){

        return;

    }else {

        for (int i = 0; i < 9 - nd->depth; i++) {

            int valid_child = 1;

            for (int j = 0; j < nd->depth -1 ; j++) {

                if (i == nd->move_interdit[j]) {  // Vérifier que le mouvement n'est pas interdit movesrobots[]

                    valid_child = 0;
                    break;

                }
            }

            if (valid_child) {

                insertInTree(nd, i, map,available_moves);

            }
        }

        for (int i = 0; i < nd->num_children; i++) {

            if (nd->children[i] != NULL) {

                auxiCompleteTree(nd->children[i], map,available_moves);

            }
        }
        return;
    }
}


void displayTree(t_node *root, int depth) {
    if (root == NULL) {
        printf("Tree is empty\n");
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

void CheminRacineFeuilleAuxiliaire(t_node *node, t_node* target, t_node*** tab) {
    if (node == NULL) return;

    for (int i = 0; i < node->num_children; i++) {
        CheminRacineFeuilleAuxiliaire(node->children[i], target, tab);
        if (node->children[i] == target) {
            (*tab)[node->depth] = target;
            CheminRacineFeuilleAuxiliaire((*tab)[0], node, tab);
        }
    }

}


t_node **CheminRacineFeuille(t_tree tree, t_node* target) {

   t_node **tab = (t_node **)malloc((tree.height + 1) * sizeof(t_node *));

   tab[0] = tree.root;
   tab[tree.height] = target;

    CheminRacineFeuilleAuxiliaire(tree.root, target, &tab);
   return tab;
}





/*void freeTree(t_node *root) {
    if (root == NULL) {
        return; // Rien à libérer
    }

    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i] != NULL) {
            if(root->num_children>0){
                freeTree(root->children[i]);
                root->children[i] = NULL;
            }
        }
    }

    if (root->children != NULL) {
        free(root->children);
        root->children = NULL;
    }

    // Libérer le tableau des mouvements interdits si alloué
    if (root->move_interdit != NULL) {
        free(root->move_interdit);
        root->move_interdit = NULL;
    }

    free(root);
}*/


char* getOrientation (int ori){
    return _orientation[ori];
}

void freeTree(t_node *root){
    if (root==NULL){
        return;
    }else {
        for (int i = 0; i < root->num_children; i++) {
            if (root->num_children > 0){
                freeTree(root->children[i]);
            }
        }
        free(root->children);
        free(root);
        return;
    }
}


void play(t_map map) {

    double tempsInitial = clock();

    srand(time(NULL));

    // Déterminez le nombre de mouvements disponibles
    int available_moves = 5; // Par défaut, 5 mouvements

    // Créer l'arbre associé à sa position
    t_tree tree = createTree(map, available_moves);

    int win = 0;

    while (1) {

        available_moves = 5;



        double debut = clock(); // temps de début
        // Trouver la feuille de plus bas coût et s'y déplacer
        t_node* leaf_node = SearchLeafMin(tree);
        double fin = clock(); // temps de fin
        double temps = fin-debut;
        printf("La fonction SearchLeafMin prend %.6f millisecondes\n", temps);

        if (leaf_node == NULL) {
            printf("Aucune feuille valide trouvee.\n");
            break;
        }

        // Afficher le mouvement choisi
        printf("Position de la feuille minimale trouvee (x,y): %d %d\n",leaf_node->loc.pos.x,leaf_node->loc.pos.y);

        // Mettre à jour la position du robot en fonction du mouvement choisi
        t_node **path = CheminRacineFeuille(tree,leaf_node);

        for (int j = 0; j<5; j++){

            if(leaf_node->cost != 0){
                move(tree.root->loc,path[j]->move);
            }else{
                printf("C'est gagne !\n");
                win = 1;
                break;
            }

        }

        // Vérifier si le robot a atteint la base
        if(win){
            break;
        }

        // Vérifier le type de terrain actuel
        int terrain_type = map.soils[tree.root->loc.pos.y][tree.root->loc.pos.x];
        int is_on_erg = (terrain_type == 2);

        // Gérer les conséquences si le robot commence sur une case erg
        if (is_on_erg) {
            printf("Le Robot commence sur un erg.\n");
            break;
        }

        if (map.soils[tree.root->loc.pos.y][tree.root->loc.pos.x] == 3){
            available_moves = 4;
        }

        freeTree(tree.root);

        break;

        // Libérer la mémoire allouée


        debut = clock(); // temps de début
        //freeTree(tree.root);
        fin = clock(); // temps de fin
        temps = fin-debut;
        printf("La fonction freeTree prend %.6f millisecondes\n", temps);

        // Créer un nouvel arbre pour le prochain mouvement
        //tree = createTree(map, available_moves,is_on_erg);
        debut = clock(); // temps de début
        //tree = createTree(map, available_moves,is_on_erg);
        fin = clock(); // temps de fin
        temps = fin-debut;
        printf("La fonction CreateTree prend %.6f millisecondes\n", temps);
    }


    // Libérer la mémoire allouée
    //freeTree(tree.root);

    double tempsFinal = clock();
    //double tempsTotal = tempsFinal-tempsInitial;
    //printf("La fonction Play prend %.6f millisecondes\n", (tempsTotal));
}
