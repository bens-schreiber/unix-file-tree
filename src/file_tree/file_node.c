#include "file_tree.h"
#include "../consts.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

file_node_t *file_node_init(file_name_t name)
{
    file_node_t *node = malloc(sizeof(file_node_t));
    assert(node != NULL);

    node->children = linked_list_init();

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
    file_node_t *child = file_node_init(name);

    // Add child to parent
    // Increment children_size
    linked_list_insert(node->children, (void *)child);
    node->children_size++;

    return child;
}

void *file_node_free(file_node_t *node)
{
    linked_list_free(node->children);
    free(node);
    node = NULL;
    return node;
}