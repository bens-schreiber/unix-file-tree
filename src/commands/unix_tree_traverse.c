#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char _traverse(const file_tree_t *tree, linked_list_t *path_string_list, linked_list_t *new_system_path)
{

    // Exhausted the path string list
    if (path_string_list->size == 0)
    {
        return 1;
    }

    const char *phrase = path_string_list->head->data;
    const file_node_t *cwd = new_system_path->tail->data;

    // Up 1 dir
    if (strcmp(phrase, "..") == 0 && cwd != tree->root)
    {
        linked_list_pop_tail(new_system_path);
    }

    // Must be navigating to a directory, find the directory in the current directory
    else if (strcmp(phrase, ".") != 0)
    {
        linked_list_node_t *iter = cwd->children->head;
        file_node_t *found = NULL;
        while (iter != NULL)
        {
            file_node_t *child = (file_node_t *)iter->data;
            if (strcmp(child->name, phrase) == 0)
            {
                found = child;
                break;
            }
            iter = iter->next;
        }

        if (found == NULL)
        {
            return 0;
        }

        linked_list_insert_tail(new_system_path, found);
    }

    linked_list_pop_head(path_string_list);
    return _traverse(tree, path_string_list, new_system_path);
}

linked_list_t *unix_tree_traverse(const file_tree_t *tree, const char *path)
{
    linked_list_t *path_string_list = linked_list_init();
    linked_list_t *new_system_path = linked_list_init();

    // If the path string starts with '/', it is absolute and we need to start from the root
    if (path[0] == '/')
    {
        linked_list_insert(new_system_path, tree->root);
    }

    // The path is relative to the current directory. Copy the current system path to the new system path
    else
    {
        linked_list_node_t *iter = system_path()->head;
        while (iter != NULL)
        {
            linked_list_insert_tail(new_system_path, iter->data);
            iter = iter->next;
        }
    }

    // turn path into a linked list
    char *path_copy = strdup(path);
    char *token = strtok(path_copy, "/");
    while (token != NULL)
    {
        char *token_copy = strdup(token);
        linked_list_insert_tail(path_string_list, token_copy);
        token = strtok(NULL, "/");
    }

    unsigned char result = _traverse(tree, path_string_list, new_system_path);

    linked_list_free(path_string_list);
    free(path_copy);

    if (result == 0)
    {
        linked_list_free(new_system_path);
        return NULL;
    }

    return new_system_path;
}