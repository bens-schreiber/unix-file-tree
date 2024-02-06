#include <stdlib.h>
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
        out_buffer[index] = '/';
        index += 1;
        for (int i = 0; i < strlen(node->name); i++)
        {
            out_buffer[index] = node->name[i];
            index += 1;
        }
        iter = iter->next;
    }
    out_buffer[index] = '\0';
}

void cd(const char *dir_name, out_buffer_t out_buffer)
{
    // /* 
    //     We are going to be given a string in the format dir1/dir2/dir3 and so on.
    //     Essentially, these are a series of directories that we need to traverse to.
    //     We will split the string by the '/' character and then traverse the tree
    //     to find the directory we are looking for.

    //     There are two cases to consider:
    //     1. The directory is inside of the current directory (i.e. it is a child of the current directory)
    //     2. The directory is in a completely different part of the tree (i.e. it is not a child of the current directory)
    // */

    // // Split the string by the '/' character
    // char *token = strtok((char *)dir_name, "/");
    // linked_list_t *new_path = linked_list_init();
    // linked_list_t *old_path = linked_list_init();
    // while (token != NULL)
    // {
        
    //     token = strtok(NULL, "/");
    // }
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