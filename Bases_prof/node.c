//
// Created by antoi on 08/11/2024.
//
#include "node.h"
#include <stddef.h>
#include <stdlib.h>

node create_node(int val,int depth){
    node nd;
    nd->value=val;
    nd->depth=depth;
    nd->enfants = malloc((9-nd->depth)*sizeof(node));
    nd->move_interdit= malloc((depth*sizeof(int)));
    return nd;
}

void delete_node(node nd){
    free(nd->enfants);
    free(nd);
}

