#include <stdio.h>
#include "map.h"
#include "tree.h"

int main() {
    t_map map;

    int n_map;

    do {
        printf("Sur quelle carte voulez-vous jouer ? Entrez un numero entre 1 et 5:\n");
        scanf("%d", &n_map);
    }while(n_map<1 || n_map>6);

    switch (n_map) {

        case 1:
            map = createMapFromFile("..\\maps\\map1.map");
            break;
        case 2:
            map = createMapFromFile("..\\maps\\map2.map");
            break;
        case 3:
            map = createMapFromFile("..\\maps\\map3.map");
            break;
        case 4:
            map = createMapFromFile("..\\maps\\map4.map");
            break;
        case 5:
            map = createMapFromFile("..\\maps\\map5.map");
            break;
        case 6:
            map = createMapFromFile("..\\maps\\map6.map");
        default:
            break;
    }

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    //printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }

    displayMap(map);

    play(map);

    return 0;



}