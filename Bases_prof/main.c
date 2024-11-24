#include <stdio.h>          // Inclusion de la bibliothèque standard d'entrées/sorties pour printf, scanf, etc.
#include <unistd.h>         // Inclusion de la bibliothèque unistd pour utiliser la fonction sleep()
#include "map.h"            // Inclusion du fichier d'en-tête "map.h" pour manipuler les cartes.
#include "tree.h"           // Inclusion du fichier d'en-tête "tree.h" pour manipuler les arbres.

int main() {
    t_map map;             // Déclaration de la variable 'map' de type 't_map', qui représente la carte de jeu.
    int n_map;             // Déclaration de la variable 'n_map', qui va stocker le numéro de la carte choisie par l'utilisateur.

    // Boucle do-while pour demander à l'utilisateur de choisir un numéro de carte entre 1 et 5, jusqu'à ce qu'une valeur valide soit entrée.
    do {
        printf("Sur quelle carte voulez-vous jouer ? Entrez un numero entre 1 et 5:\n");   // Affiche un message demandant à l'utilisateur de choisir une carte.
        scanf("%d", &n_map);  // Lecture de l'entrée de l'utilisateur, le numéro de carte.
    } while (n_map < 1 || n_map > 5); // Tant que le numéro de carte est hors de la plage 1-5, répéter la demande.

    // Switch-case pour charger la carte correspondante selon le numéro choisi par l'utilisateur.
    switch (n_map) {

        case 1:
            map = createMapFromFile("..\\maps\\map1.map");  // Charge la carte 1 depuis le fichier "map1.map".
            break;                                          // Quitte le switch après avoir chargé la carte.
        case 2:
            map = createMapFromFile("..\\maps\\map2.map");  // Charge la carte 2 depuis le fichier "map2.map".
            break;
        case 3:
            map = createMapFromFile("..\\maps\\map3.map");  // Charge la carte 3 depuis le fichier "map3.map".
            break;
        case 4:
            map = createMapFromFile("..\\maps\\map4.map");  // Charge la carte 4 depuis le fichier "map4.map".
            break;
        case 5:
            map = createMapFromFile("..\\maps\\map5.map");  // Charge la carte 5 depuis le fichier "map5.map".
            break;
        default:
            break;  // Cas par défaut (non utilisé ici, car l'entrée de l'utilisateur est vérifiée avant).
    }

    // Affiche les dimensions de la carte créée (nombre de lignes et de colonnes).
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);

    // Affiche le contenu de la carte (les valeurs dans 'soils') sous forme de tableau 2D.
    for (int i = 0; i < map.y_max; i++)  // Parcourt chaque ligne de la carte.
    {
        for (int j = 0; j < map.x_max; j++)  // Parcourt chaque colonne de la carte.
        {
            printf("%d ", map.soils[i][j]);  // Affiche la valeur de la cellule dans le tableau 'soils' à la position (i,j).
        }
        printf("\n");  // Saut de ligne après chaque ligne de la carte.
    }

    sleep(5);

    // Affiche les coûts associés à chaque case de la carte, formaté pour être aligné à gauche avec 5 espaces.
    for (int i = 0; i < map.y_max; i++)  // Parcourt chaque ligne du tableau des coûts.
    {
        for (int j = 0; j < map.x_max; j++)  // Parcourt chaque colonne du tableau des coûts.
        {
            printf("%-5d ", map.costs[i][j]);  // Affiche le coût de la case avec une largeur de 5 caractères alignés à gauche.
        }
        printf("\n");  // Saut de ligne après chaque ligne des coûts.
    }

    sleep(5);

    displayMap(map);   // Appel de la fonction 'displayMap' pour afficher la carte visuellement (fonction non fournie dans ce code).

    sleep(5);

    play(map);         // Appel de la fonction 'play' pour démarrer le jeu avec la carte chargée (fonction non fournie dans ce code).

    return 0;          // Retourne 0 pour indiquer que le programme s'est exécuté correctement.
}