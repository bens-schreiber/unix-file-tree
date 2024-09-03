#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "../tree_serializer/tree_serializer.h"

/// @brief Asserts that the condition is true, and if it is not, sets the out_buffer to the error message and returns.
#define out_error(condition, out_buffer, error) \
    if (condition)                              \
    {                                           \
        strcpy(out_buffer, error);              \
        return;                                 \
    }

/// @brief The system path that the terminal is currently operating on.
static linked_list_t *_system_path = NULL;

const linked_list_t *system_path()
{
    return _system_path;
}

void path_init(const file_tree_t *tree)
{
    _system_path = linked_list_init();
    linked_list_insert_head(_system_path, tree->root);
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

void cd(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{

    // Go to root on no path
    if (path == NULL || strlen(path) == 0)
    {
        path = "/";
    }

    linked_list_t *new_system_path = unix_tree_traverse(tree, path);
    out_error(new_system_path == NULL, out_buffer, "err: directory not found");
    out_error(((file_node_t *)new_system_path->tail->data)->is_dir == 0, out_buffer, "err: not a directory");
    linked_list_free(_system_path);
    _system_path = new_system_path;
}

void ls(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{

    // Assume the current directory if no path is given
    if (path == NULL || strlen(path) == 0)
    {
        path = ".";
    }

    linked_list_t *dir_path = unix_tree_traverse(tree, path);
    out_error(dir_path == NULL, out_buffer, "err: directory not found");
    out_error(((file_node_t *)dir_path->tail->data)->is_dir == 0, out_buffer, "err: not a directory");

    file_node_t *dir = (file_node_t *)dir_path->tail->data;
    linked_list_node_t *iter = dir->children->head;
    int index = 0;
    while (iter != NULL)
    {
        file_node_t *child = (file_node_t *)iter->data;

        // append "D " to the beginning of the name if it is a directory, "F " otherwise
        strcpy(&out_buffer[index], child->is_dir ? "D " : "F ");
        index += 2;

        for (int i = 0; i < strlen(child->name); i++)
        {
            out_buffer[index] = child->name[i];
            index += 1;
        }
        out_buffer[index] = '\t';
        index += 1;
        iter = iter->next;
    }

    strcpy(&out_buffer[index], "\0");
}

void mkdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{

    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no directory specified");
    linked_list_t *dir_path = unix_tree_traverse_find_closest(tree, path);
    out_error(dir_path == NULL, out_buffer, "err: directory not found");

    linked_list_t *path_string_list = linked_list_init();

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, "/");
    while (token != NULL)
    {
        char *token_copy = strdup(token);
        linked_list_insert_tail(path_string_list, token_copy);
        token = strtok(NULL, "/");
    }

    file_node_t *parent = (file_node_t *)dir_path->tail->data;
    out_error(parent->is_dir == 0, out_buffer, "err: tried to create directory in file");

    const char *name = path_string_list->tail->data;
    const unsigned char exists = strcmp(parent->name, name) == 0;

    if (!exists)
    {
        (void)file_tree_add_child(tree, parent, name, 1);
    }

    linked_list_free(path_string_list);
    free(path_copy);
    linked_list_free(dir_path);

    out_error(exists, out_buffer, "err: name already exists");
}

void rmdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{
    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no directory specified");
    linked_list_t *dir_path = unix_tree_traverse(tree, path);
    out_error(dir_path == NULL, out_buffer, "err: directory not found");

    file_node_t *node = (file_node_t *)dir_path->tail->data;

    out_error(!node->is_dir, out_buffer, "err: tried to delete file, not directory");
    out_error(node->children->size > 0, out_buffer, "err: directory not empty");

    // find parent
    file_node_t *parent = NULL;
    linked_list_node_t *iter = dir_path->head;
    while (iter != NULL && iter->data != node)
    {
        parent = (file_node_t *)iter->data;
        iter = iter->next;
    }

    file_tree_delete_child(tree, parent, node);
}

void creat(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{
    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no file specified");
    linked_list_t *dir_path = unix_tree_traverse_find_closest(tree, path);
    out_error(dir_path == NULL, out_buffer, "err: directory not found");

    linked_list_t *path_string_list = linked_list_init();

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, "/");
    while (token != NULL)
    {
        char *token_copy = strdup(token);
        linked_list_insert_tail(path_string_list, token_copy);
        token = strtok(NULL, "/");
    }

    file_node_t *parent = (file_node_t *)dir_path->tail->data;
    out_error(parent->is_dir == 0, out_buffer, "err: tried to create file in file");

    const char *name = path_string_list->tail->data;
    const unsigned char exists = strcmp(parent->name, name) == 0;

    if (!exists)
    {
        (void)file_tree_add_child(tree, parent, name, 0);
    }

    linked_list_free(path_string_list);
    free(path_copy);
    linked_list_free(dir_path);

    out_error(exists, out_buffer, "err: name already exists");
}

void rm(out_buffer_t out_buffer, file_tree_t *tree, const char *path)
{
    out_error(path == NULL || strlen(path) == 0, out_buffer, "err: no file specified");
    linked_list_t *dir_path = unix_tree_traverse(tree, path);
    out_error(dir_path == NULL, out_buffer, "err: file not found");

    file_node_t *node = (file_node_t *)dir_path->tail->data;

    // find parent
    file_node_t *parent = NULL;
    linked_list_node_t *iter = dir_path->head;
    while (iter != NULL && iter->data != node)
    {
        parent = (file_node_t *)iter->data;
        iter = iter->next;
    }

    out_error(node->is_dir, out_buffer, "err: tried to delete directory, not file");
    file_tree_delete_child(tree, parent, node);
}

void save(out_buffer_t out_buffer, file_tree_t *tree, const char *filename)
{
    out_error(filename == NULL || strlen(filename) == 0, out_buffer, "err: no file specified");
    tree_serialize_to_file(tree, filename);
}

file_tree_t *reload(out_buffer_t out_buffer, file_tree_t *tree, const char *filename)
{

    if (filename == NULL || strlen(filename) == 0)
    {
        strcpy(out_buffer, "err: no file specified");
        return tree;
    }

    file_tree_t *new_tree = tree_deserialize_from_file(filename);
    if (new_tree == NULL)
    {
        strcpy(out_buffer, "err: could not load file");
        return tree;
    }
    file_tree_free(tree);
    path_free();
    path_init(new_tree);
    return new_tree;
}