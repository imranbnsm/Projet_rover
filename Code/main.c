#include <stdio.h>
#include <unistd.h>         // Inclusion de la bibliothèque unistd pour utiliser la fonction sleep()
#include "map.h"
#include "tree.h"

int main() {
    t_map map;
    int n_map;             // Déclaration de la variable 'n_map', qui va stocker le numéro de la carte choisie par l'utilisateur.

    // Boucle do-while pour demander à l'utilisateur de choisir un numéro de carte entre 1 et 5, jusqu'à ce qu'une valeur valide soit entrée.
    do {
        printf("Sur quelle carte voulez-vous jouer ? Entrez un numero entre 1 et 5:\n");
        scanf("%d", &n_map);
    } while (n_map < 1 || n_map > 5);

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

    printf("\n");

    // Affiche les dimensions de la carte créée (nombre de lignes et de colonnes).
    printf("Map created with dimensions %d x %d\n\n", map.y_max, map.x_max);

    // Affiche le contenu de la carte (les valeurs dans 'soils') sous forme de tableau 2D.
    for (int i = 0; i < map.y_max; i++)  // Parcourt chaque ligne de la carte.
    {
        for (int j = 0; j < map.x_max; j++)  // Parcourt chaque colonne de la carte.
        {
            printf("%d ", map.soils[i][j]);  // Affiche la valeur de la cellule dans le tableau 'soils' à la position (i,j).
        }
        printf("\n");  // Saut de ligne après chaque ligne de la carte.
    }

    printf("\n\n");

    sleep(2);

    // Affiche les coûts associés à chaque case de la carte, formaté pour être aligné à gauche avec 5 espaces.

    printf("Carte avec couts associes a chaque case:\n\n");

    for (int i = 0; i < map.y_max; i++)  // Parcourt chaque ligne du tableau des coûts.
    {
        for (int j = 0; j < map.x_max; j++)  // Parcourt chaque colonne du tableau des coûts.
        {
            printf("%-5d ", map.costs[i][j]);  // Affiche le coût de la case avec une largeur de 5 caractères alignés à gauche.
        }
        printf("\n");  // Saut de ligne après chaque ligne des coûts.
    }

    printf("\n\n");

    sleep(4);

    printf("Carte visuelle :\n\n");

    displayMap(map);   // Appel de la fonction 'displayMap' pour afficher la carte visuellement.

    printf("\n");

    sleep(4);

    play(map);         // Appel de la fonction 'play' pour démarrer le jeu avec la carte chargée.

    return 0;
}