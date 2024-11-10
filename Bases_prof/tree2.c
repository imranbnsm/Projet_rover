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
