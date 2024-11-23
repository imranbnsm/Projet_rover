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
#include <time.h>

t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
t_move *movesrobot;
int length_path=0;

//pour le move en string appeler fonction movesAstring dans moves.c

void getMoves(int is_on_erg){
    movesrobot = (t_move *)malloc(9 * sizeof(t_move));

    srand(time(NULL));
    int dispo_moves[] = {22, 15, 7, 7, 21, 21, 7};
    int total_moves=100;

    for (int i = 0; i < 9; i++) {
        int r = rand() % total_moves;  // Sélectionne un numéro aléatoire
        int type = 0;

        // Parcourir les types de mouvements pour trouver celui correspondant
        while (r >= dispo_moves[type]) {
            r -= dispo_moves[type];
            type++;
        }

        // Ajouter le mouvement choisi à la liste
        movesrobot[i] = moves[type];

        // Mettre à jour les disponibilités
        dispo_moves[type]--;
        total_moves--;

    }

    // Si le robot est sur une case erg, adapter les mouvements
    if (is_on_erg) {
        for (int i = 0; i < 9; i++) {
            switch (movesrobot[i]) {
                case F_10:
                    // Ne fait rien
                    movesrobot[i] = NONE; // Ou une autre valeur représentant un mouvement invalide
                    break;
                case F_20:
                    // N'avance que de 10m
                    movesrobot[i] = F_10;
                    break;
                case F_30:
                    // N'avance que de 20m
                    movesrobot[i] = F_20;
                    break;
                case B_10:
                    // Ne fait rien
                    movesrobot[i] = NONE; // Ou une autre valeur représentant un mouvement invalide
                    break;
                case U_TURN:
                    // Ne pas tourner
                    if (rand()%2==1) {
                        movesrobot[i] = T_RIGHT; // Ou une autre valeur représentant un mouvement invalide
                    } else {
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

    do {
        randomPos.x = rand() % map.x_max;
        randomPos.y = rand() % map.y_max;
    } while ( (map.costs[randomPos.y][randomPos.x] == 0 ) || (map.costs[randomPos.y][randomPos.x] > 10000 ) ); // Vérifier les coordonnées

    return randomPos;
}

t_orientation generateRandomOrientation() {
    return (t_orientation)(rand() % 4);
}


t_tree createTree(t_map map, int available_moves, int is_on_erg, t_localisation robot) {
    t_tree tree;
    tree.height = available_moves;
    getMoves(is_on_erg); // Passer l'état de la case erg
    int cost = map.costs[robot.pos.y][robot.pos.x];
    tree.root = createNode(robot, cost, 0);
    completeTree(&tree, map, available_moves);
    return tree;
}


void insertInTree(t_node *nd, int i_move, t_map map, int available_moves) {

    if (nd != NULL) {
        // Vérifiez si l'indice de mouvement est valide

        t_localisation new_pos = move(nd->loc, movesrobot[i_move]);

        if (isValidLocalisation(new_pos.pos, map.x_max, map.y_max) && nd->cost < 10000) {
            int cost = map.costs[new_pos.pos.y][new_pos.pos.x];
            t_node *nd_child = createNode(new_pos, cost, nd->depth + 1);

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

    if (nd==NULL || nd->depth<1 || nd->depth >= available_moves){

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


t_node **CheminRacineFeuille(t_tree tree) {

    t_node *target = SearchLeafMin(tree);
    t_node **tab = (t_node **)malloc((target->depth) * sizeof(t_node *));


   tab[0] = tree.root;
   tab[target->depth-1] = target;
   length_path=target->depth;
   CheminRacineFeuilleAuxiliaire(tree.root, target, &tab);
   return tab;
}


void freeTree(t_node *root) {
    if (root == NULL) {
        return; // Rien à libérer
    }
    printf("%d ",root->num_children);
    // Libération récursive des enfants
    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i]!=NULL){
            freeTree(root->children[i]);
        }
    }

    // Libération des structures internes du nœud
    free(root->children);      // Libère le tableau des enfants
    free(root->move_interdit); // Libère le tableau des mouvements interdits

    // Libération du nœud lui-même
    free(root);
}


char* getOrientation (int ori){
    return _orientation[ori];
}

void play(t_map map) {
    double tempsInitial = clock();
    t_tree tree;
    t_localisation robot;

    // Faire apparaître le robot à un endroit de la carte
    robot.pos = generateRandomPosition(map);
    robot.ori = generateRandomOrientation();
    printf("Robot initialise a la position (%d, %d) avec orientation %s.\n", robot.pos.x, robot.pos.y, getOrientation(robot.ori));

    // Vérifiez le type de terrain initial
    int initial_terrain_type = map.soils[robot.pos.y][robot.pos.x];

    // Déterminez le nombre de mouvements disponibles
    int available_moves = 5; // Par défaut, 5 mouvements
    if (initial_terrain_type == REG) {
        available_moves = 4; // Réduit à 4 mouvements si sur REG
    }

    // Créer l'arbre associé à sa position
    double debut = clock(); // temps de début
    tree = createTree(map, available_moves, initial_terrain_type == ERG,robot); // Passer si sur erg
    double fin = clock(); // temps de fin
    double temps = fin-debut;
    printf("La fonction CreateTree prend %.6f secondes\n", temps / CLOCKS_PER_SEC);


    while (1) {
        // Trouver la feuille de plus bas coût et s'y déplacer
        displayTree(tree.root,0);
        debut = clock(); // temps de début
        t_node ** path = CheminRacineFeuille(tree);
        fin = clock(); // temps de fin
        temps = fin-debut;
        printf("La fonction CheminRacineFeuille prend %.6f secondes\n", temps / CLOCKS_PER_SEC);
        
        // Vérifier le type de terrain actuel
        int terrain_type = map.soils[robot.pos.y][robot.pos.x];
        int is_on_erg = (terrain_type == 2);

        for (int i=0 ; i<length_path ; i++) {
            // Afficher le mouvement choisi
            printf("Mouvement choisi: %s\n", getMoveAsString(path[i]->move));

            // Mettre à jour la position du robot en fonction du mouvement choisi
            robot = move(robot, path[i]->move);
        }

        // Vérifier si le robot a atteint la base
        if (map.costs[robot.pos.y][robot.pos.x] == 0) {
            printf("C'est gagne !\n");
            break;
        }



        // Libérer la mémoire allouée
        freeTree(tree.root);
        // Créer un nouvel arbre pour le prochain mouvement
        tree = createTree(map, available_moves,is_on_erg,robot);
    }

    // Libérer la mémoire allouée
    freeTree(tree.root);
    double tempsFinal = clock();
    double tempsTotal = tempsFinal-tempsInitial;
    printf("La fonction Play prend %.6f secondes\n", (tempsTotal / CLOCKS_PER_SEC));
}
