//
// Created by antoi on 08/11/2024.
//

#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H
#include "moves.h"
struct s_node
{
    int value;
    struct s_node** children;
    int depth;
    int* move_interdit;
    t_move move;
};
typedef struct s_node *node;

node create_node(int val, int depth);
void delete_node(node nd);

#endif //UNTITLED1_NODE_H
