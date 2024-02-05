#include "file-tree.h"
#include "../consts.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

file_node_t *file_node_init(file_name_t name)
{
    file_node_t *node = malloc(sizeof(file_node_t));
    assert(node != NULL);

    // Initialize children to NULL
    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        node->children[i] = NULL;
    }

    // Copy name
    strcpy(node->name, name);
    for (int i = 0; i < MAX_FILE_NAME_LENGTH; i++)
    {
        node->name[i] = name[i];
    }

    // No children initially
    node->children_size = 0;

    return node;
}

file_node_t *file_node_add_child(file_node_t *node, file_name_t name)
{
    assert(node->children_size < MAX_FILE_CHILDREN);
    file_node_t *child = file_node_init(name);

    // Add child to parent
    // Increment children_size
    node->children[node->children_size++] = child;
    return child;
}

void *file_node_free(file_node_t *node)
{

    // Free children
    for (int i = 0; i < node->children_size; i++)
    {
        file_node_free(node->children[i]);
        node->children[i] = NULL;
    }

    free(node);
    node = NULL;
    return node;
}