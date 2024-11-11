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
        nd=nd->children[list_moves[i]];
    }
    t_localisation new_pos = move(nd->loc,moves[list_moves[length_moves-1]]);
    if (isValidLocalisation(new_pos.pos,map.x_max,map.y_max)){
        int cost=map.costs[new_pos.pos.y][new_pos.pos.x];
        t_node *nd_child= createNode(new_pos, cost, nd->depth+1);
        addChild(nd, nd_child);
        nd_child->move=moves[list_moves[length_moves-1]];
        for(int j=0;j<length_moves;j++){
            nd_child->move_interdit[j]=list_moves[j];
        }
    } else {
        //fin de partie
    }
}
void completeTree (t_tree *tree, t_map map){
    t_node *nd=tree->root;
    t_move moves[] = { F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};
    int j=0;
    for (int i=0;i<9;i++){
        for (int l=0;l<9-nd->depth;l++) {
            if (nd->depth == 0) {
                int list_moves[] = {l};
                insertInTree(tree, list_moves, 1, map);
            } else if (moves[nd->move_interdit[j]] != moves[l] && moves[l] != nd->move) {
                int list_moves[nd->depth + 2];
                for (int k = 0; k < nd->depth; k++) {
                    list_moves[k] = nd->move_interdit[k];
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

