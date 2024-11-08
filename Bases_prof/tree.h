//
// Created by antoi on 08/11/2024.
//

#ifndef PROJET_ROVER_TREE_H
#define PROJET_ROVER_TREE_H
struct s_node
{
    int value;
    struct s_node   *left, *right;
    int depth;
};
typedef struct s_node *node;

struct s_tree {
    node root;
};
typedef struct s_tree *tree;

tree create_tree();
void add_node(int val);
node search_node(int val);
node delete_node(int val);

#endif //PROJET_ROVER_TREE_H
