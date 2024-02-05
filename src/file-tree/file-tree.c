#include "file-tree.h"
#include "../consts.h"
#include <stdlib.h>
#include <assert.h>

file_tree_t *file_tree_init()
{
    file_tree_t *tree = malloc(sizeof(file_tree_t));
    assert(tree != NULL);

    // Initialize root node
    tree->root = file_node_init(TREE_ROOT_NAME);
    tree->size = 1;
    return tree;
}

void *file_tree_free(file_tree_t *tree)
{
    tree->root = file_node_free(tree->root);
    free(tree);
    return NULL;
}

void *file_tree_free_child(file_tree_t *tree, file_node_t *parent, file_node_t *node)
{
    node = file_node_free(node);
    parent->children_size--;

    
    
    tree->size--;
    return node;
}
