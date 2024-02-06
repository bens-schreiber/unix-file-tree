#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

/// @brief Asserts that the condition is true, and if it is not, sets the out_buffer to the error message and returns.
#define out_error(condition, out_buffer, error) \
    if (condition)                              \
    {                                           \
        strcpy(out_buffer, error);              \
        return;                                 \
    }

static linked_list_t *_system_path = NULL;

const linked_list_t *system_path()
{
    return _system_path;
}

void path_init(const file_tree_t *tree)
{
    _system_path = linked_list_init();
    linked_list_insert(_system_path, tree->root);
}

void path_free()
{
    linked_list_free(_system_path);
}

void pwd(out_buffer_t out_buffer)
{
    linked_list_node_t *iter = _system_path->head;
    int index = 0;
    while (iter != NULL)
    {
        file_node_t *node = (file_node_t *)iter->data;
        for (int i = 0; i < strlen(node->name); i++)
        {
            out_buffer[index] = node->name[i];
            index += 1;
        }
        iter = iter->next;
        out_buffer[index] = '/';
        index += 1;
    }
    out_buffer[index] = '\0';
}

void cd(out_buffer_t out_buffer, file_tree_t *tree, const char *dir_name)
{
    out_error(dir_name == NULL || strlen(dir_name) == 0, out_buffer, "err: no directory specified\n");
    linked_list_t *new_system_path = unix_tree_traverse(tree, dir_name);
    out_error(new_system_path == NULL, out_buffer, "err: directory not found\n");
    
    linked_list_free(_system_path);
    _system_path = new_system_path;
}

void ls(out_buffer_t out_buffer)
{
    file_node_t *node = (file_node_t *)_system_path->tail->data;
    linked_list_node_t *iter = node->children->head;
    int index = 0;
    while (iter != NULL)
    {
        file_node_t *child = (file_node_t *)iter->data;
        for (int i = 0; i < strlen(child->name); i++)
        {
            out_buffer[index] = child->name[i];
            index += 1;
        }
        out_buffer[index] = '\n';
        index += 1;

        if (iter->next == NULL)
        {
            break;
        }
        iter = iter->next;
    }
    out_buffer[index] = '\0';
}

// TODO: Do we want absolute path creation?
void mkdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{
    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no directory specified\n");

    // If the path starts with no slash, it is just the current directory
    if (strstr(path, "/") == NULL)
    {
        file_node_t *node = (file_node_t *)_system_path->tail->data;

        // check if the directory already exists
        linked_list_t *child = unix_tree_traverse(node, path);
        linked_list_free(child);
        out_error(child != NULL, out_buffer, "err: directory already exists\n");
        child = NULL;

        file_tree_add_child(tree, node, path);
        return;
    }
}

// TODO: Do we want absolute path deletion?
void rmdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{
    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no directory specified\n");

    // If the path starts with no slash, it is just the current directory
    if (strstr(path, "/") == NULL)
    {
        file_node_t *parent = (file_node_t *)_system_path->tail->data;
        linked_list_t *child_to_remove = unix_tree_traverse(parent, path);

        out_error(child_to_remove == NULL, out_buffer, "err: directory not found\n");

        file_node_t *child = (file_node_t *)child_to_remove->tail->data;

        file_tree_delete_child(tree, parent, child);

        linked_list_free(child_to_remove);
    }
}