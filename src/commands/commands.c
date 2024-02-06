#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

static linked_list_t *_system_path = NULL;

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

void cd(file_tree_t *tree, const char *dir_name)
{
    assert(dir_name != NULL && strlen(dir_name) != 0);

    if (strcmp(dir_name, ".") == 0)
    {
        return;
    }

    if (strcmp(dir_name, "..") == 0)
    {
        linked_list_pop_tail(_system_path);
        return;
    }

    if (strcmp(dir_name, "/") == 0)
    {
        linked_list_free(_system_path);
        _system_path = linked_list_init();
        linked_list_insert(_system_path, tree->root);
        return;
    }

    linked_list_t *path = NULL;

    // If the path is relative (ie ./dir) we need to search from the current directory
    // If the path is absolute (ie /dir) we need to search from the root
    if (dir_name[0] == '.' && dir_name[1] == '/')
    {
        dir_name += 2;
        path = tree_search_with_path((file_node_t *)_system_path->tail->data, dir_name);
    }
    else
    {
        path = tree_search_with_path(tree->root, dir_name);
    }

    if (path == NULL)
    {
        return;
    }

    linked_list_free(_system_path);
    _system_path = path;
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
void mkdir(file_tree_t *tree, const char *path)
{
    assert(path != NULL && strlen(path) != 0);
    // If the path starts with no slash, it is just the current directory
    if (strstr(path, "/") == NULL)
    {
        file_node_t *node = (file_node_t *)_system_path->tail->data;
        file_tree_add_child(tree, node, path);
        return;
    }
}

// TODO: Do we want absolute path creation?
void rmdir(file_tree_t *tree, const char *path)
{
    assert(path != NULL && strlen(path) != 0);
    // If the path starts with no slash, it is just the current directory
    if (strstr(path, "/") == NULL)
    {
        file_node_t *parent = (file_node_t *)_system_path->tail->data;
        linked_list_t *child_to_remove = tree_search_with_path(parent, path);
        assert(child_to_remove != NULL);
        file_node_t *child = (file_node_t *)child_to_remove->tail->data;

        file_tree_delete_child(tree, parent, child);

        linked_list_free(child_to_remove);
    }
}