#include "file-tree.h"
#include <stdlib.h>
#include <assert.h>

file_tree_t *file_tree_init()
{
    file_tree_t *tree = malloc(sizeof(file_tree_t));
    assert(tree != NULL);

    // Initialize root node, with name "/"
    tree->root = file_node_init("/");
    tree->size = 1;
    return tree;
}

void *file_tree_free(file_tree_t *tree)
{
    tree->root = file_node_free(tree->root);
    free(tree);
    return NULL;
}