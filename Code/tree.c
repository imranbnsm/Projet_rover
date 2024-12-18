//
// Created by Imran on 10/11/2024.
//
#include <unistd.h>
#include <stdio.h>
#include "tree.h"
#include "loc.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "moves.h"

t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
t_move *movesrobot;
int length_path=0;


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
            int r2 = rand() % 2;
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
                    if(r2 == 0){
                        movesrobot[i] = T_RIGHT;
                    }else{
                        movesrobot[i] = T_LEFT;
                    }

                    break;
                default:
                    break;
            }
        }
    }
}




t_position generateRandomPosition(t_map map) {
    t_position randomPos;

    // Générer une position aléatoire valide (coût dans la carte entre 1 et 10000)
    do {
        randomPos.x = rand() % map.x_max;
        randomPos.y = rand() % map.y_max;
    } while ((map.costs[randomPos.y][randomPos.x] == 0) || (map.costs[randomPos.y][randomPos.x] > 10000));

    return randomPos;
}

t_orientation generateRandomOrientation() {
    // Générer une orientation aléatoire (Nord, Est, Sud, Ouest)
    return (t_orientation)(rand() % 4);
}


t_tree createTree(t_map map, int available_moves, t_localisation robot) {
    t_tree tree;

    // Initialiser l'arbre avec le nombre de mouvements disponibles
    tree.height = available_moves;

    // Générer une position et une orientation aléatoire pour le robot

    int cost = map.costs[robot.pos.y][robot.pos.x];
    tree.root = createNode(robot, cost, 0); // Créer la racine de l'arbre

    // Vérifier si le robot commence sur un erg
    int is_on_erg = (map.soils[robot.pos.y][robot.pos.x] == 2);

    // Générer les mouvements possibles en fonction du type de terrain
    getMoves(is_on_erg);

    // Afficher les informations initiales du robot
    printf("Robot initialise a la position (%d, %d) avec orientation %s.\n\n", robot.pos.x, robot.pos.y, getOrientation((robot.ori)));

    // Compléter l'arbre avec les mouvements disponibles
    completeTree(&tree, map, available_moves);

    return tree;
}


void insertInTree(t_node *nd, int i_move, t_map map, int available_moves) {
    if (nd != NULL) {
        // Vérifier si la profondeur maximale de l'arbre a été atteinte
        if (nd->depth >= available_moves) {
            return;
        }

        int profondeur = nd->depth;

        // Calculer la nouvelle position après le mouvement
        t_localisation new_pos = move(nd->loc, movesrobot[i_move]);

        // Vérifier que la nouvelle localisation est valide
        if (isValidLocalisation(new_pos.pos, map.x_max, map.y_max) && nd->cost < 10000) {
            int cost = map.costs[new_pos.pos.y][new_pos.pos.x];
            t_node *nd_child = createNode(new_pos, cost, profondeur + 1);

            if (nd_child == NULL) {
                printf("Error: Failed to create child node.\n");
                return;
            }

            // Ajouter le nouveau noeud comme enfant du noeud actuel
            addChild(nd, nd_child);
            nd_child->move = movesrobot[i_move];

            // Propager les mouvements interdits vers l'enfant
            if (nd->move_interdit != NULL) {
                for (int j = 0; j < profondeur - 1; j++) {
                    if (profondeur >= 1) {
                        nd_child->move_interdit[j] = nd->move_interdit[j];
                    }
                }
            }

            // Ajouter le mouvement actuel à la liste des mouvements interdits
            nd_child->move_interdit[profondeur - 1] = i_move;
            nd_child->depth = profondeur + 1;
        }
    }
}


void completeTree(t_tree *tree, t_map map, int available_moves) {
    if (tree != NULL && tree->root != NULL) {
        t_node *nd = tree->root;

        // Ajouter les enfants du noeud racine en fonction des mouvements disponibles
        for (int l = 0; l < 9; l++) {
            insertInTree(nd, l, map, available_moves);
        }

        // Compléter récursivement l'arbre en parcourant les enfants
        for (int k = 0; k < nd->num_children; k++) {
            auxiCompleteTree(nd->children[k], map, available_moves);
        }
    }
}

void auxiCompleteTree(t_node *nd, t_map map, int available_moves) {
    if (nd == NULL || nd->depth < 1 || nd->depth == 5) {
        return;
    } else {
        // Ajouter les enfants en vérifiant les mouvements valides
        for (int i = 0; i < 9 - nd->depth; i++) {
            int valid_child = 1;

            // Vérifier si le mouvement est interdit
            for (int j = 0; j < nd->depth - 1; j++) {
                if (i == nd->move_interdit[j]) {
                    valid_child = 0;
                    break;
                }
            }

            // Si le mouvement est valide, ajouter l'enfant
            if (valid_child) {
                insertInTree(nd, i, map, available_moves);
            }
        }

        // Compléter récursivement l'arbre pour les enfants
        for (int i = 0; i < nd->num_children; i++) {
            if (nd->children[i] != NULL) {
                auxiCompleteTree(nd->children[i], map, available_moves);
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


// Chemin depuis la racine vers cette feuille.

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

   t_node **tab = (t_node **)malloc((target->depth) * sizeof(t_node *));
   length_path=target->depth;
   tab[0] = tree.root;
   tab[length_path-1] = target;

    CheminRacineFeuilleAuxiliaire(tree.root, target, &tab);
   return tab;
}


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
    t_localisation robot;
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();

    printf("ETAPE 1\n\n");

    // Créer l'arbre associé à sa position
    t_tree tree = createTree(map, available_moves, robot);

    int win = 0;
    int loose = 0;
    int nb_phases = 1;

    while (1) {

        available_moves = 5;
        nb_phases++;

        double debut = clock(); // temps de début
        // Trouver la feuille de plus bas coût et s'y déplacer
        t_node* leaf_node = SearchLeafMin(tree);
        double fin = clock(); // temps de fin
        double temps = fin-debut;
        printf("La fonction SearchLeafMin prend %.6f millisecondes\n\n", temps);

        if (leaf_node == NULL) {
            printf("Aucune feuille valide trouvee.\n");
            break;
        }

        // Afficher le mouvement choisi
        printf("Position de la feuille minimale trouvee (x,y): %d,%d\n\n",leaf_node->loc.pos.x,leaf_node->loc.pos.y);

        // Mettre à jour la position du robot en fonction du mouvement choisi
        t_node **path = CheminRacineFeuille(tree,leaf_node);

        printf("Chemin vers la feuille minimale :\n");

        for (int j = 0; j<length_path; j++){

            if(path[j]->cost != 0){
                robot=move(robot,path[j]->move);
                printf("Case numero %d: %d,%d\n",j,path[j]->loc.pos.x,path[j]->loc.pos.y);
                printf("Cout de la case %d: %d\n",j,path[j]->cost);
                printf("Mouvement de la case numero %d: %s\n\n",j, getMoveAsString(path[j]->move));
            }else if(!isValidLocalisation(path[j]->loc.pos,map.x_max,map.y_max)){
                loose = 1;
                break;
            }else{
                printf("Case numero %d: %d,%d cout : %d\n",j,path[j]->loc.pos.x,path[j]->loc.pos.y,path[j]->cost);
                printf("Cout de la case %d: %d\n",j,path[j]->cost);
                printf("C'est gagne !\n");
                printf("ROVER a atteint la base en %d phases.\n\n",nb_phases-1);
                win = 1;
                break;
            }

        }

        if(loose){
            printf("ROVER est sorti de la carte, c'est perdu !\n");
            break;
        }

        // Vérifier si le robot a atteint la base
        if(win){
            break;
        }

        if (map.soils[tree.root->loc.pos.y][tree.root->loc.pos.x] == 3){
            available_moves = 4;
        }

        debut = clock(); // temps de début
        freeTree(tree.root);
        fin = clock(); // temps de fin
        temps = fin-debut;
        printf("La fonction freeTree prend %.6f millisecondes\n\n\n\n", temps);

        sleep(2);

        printf("ETAPE %d\n\n",nb_phases);

        // Créer un nouvel arbre pour le prochain mouvement
        debut = clock(); // temps de début
        tree = createTree(map, available_moves, robot);
        fin = clock(); // temps de fin
        temps = fin-debut;
        printf("La fonction CreateTree prend %.6f millisecondes\n\n", temps);
    }

    freeTree(tree.root);

    double tempsFinal = clock();
    double tempsTotal = tempsFinal-tempsInitial-2000*(nb_phases-2);
    printf("La fonction Play prend %.6f millisecondes\n", (tempsTotal));
}
