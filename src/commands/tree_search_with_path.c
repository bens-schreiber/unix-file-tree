#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// @return 0 if the old path is found in the new path, 1 otherwise.
unsigned char _search(const file_node_t *node, linked_list_t *old_path, linked_list_t *new_path)
{
    if (node == NULL)
    {
        return 0;
    }

    // We've exhausted the old path, so we've found it.
    if (old_path->head == NULL)
    {
        return 1;
    }

    // We need to find the first node that is a child of node and has the same name as old_head->name
    const file_node_t *old_head = old_path->head->data;

    linked_list_node_t *iter = node->children->head;
    while (iter != NULL)
    {
        file_node_t *child = (file_node_t *)iter->data;
        if (strcmp(child->name, old_head->name) == 0)
        {
            linked_list_insert(new_path, child);
            linked_list_pop_head(old_path);
            return _search(child, old_path, new_path);
        }
        iter = iter->next;
    }

    return 0;
}

linked_list_t *tree_search_with_path(const file_node_t *node, const char *path)
{
    linked_list_t *old_path = linked_list_init();
    linked_list_t *new_path = linked_list_init();

    // turn path into a linked list
    char *token = strtok((char *)path, "/");

    while (token != NULL)
    {
        linked_list_insert(old_path, token);
        token = strtok(NULL, "/");
    }

    unsigned char result = _search(node, old_path, new_path);

    linked_list_free(old_path);

    if (result)
    {
        linked_list_insert(new_path, node);
        return new_path;
    }

    linked_list_free(new_path);

    return NULL;
}