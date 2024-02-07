#include "tree_serializer.h"
#include "../consts.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static char *_string_buffer;
static int _string_buffer_index;

void preorder_traversal(const file_node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    // Append a delimiter which will be used to know if a node is a directory or file
    _string_buffer[_string_buffer_index++] = node->is_dir ? TREE_SERIALIZE_DIR_DELIMITER : TREE_SERIALIZE_FILE_DELIMITER;

    // Add the name of the node to the string buffer
    strcpy(_string_buffer + _string_buffer_index, node->name);
    _string_buffer_index += strlen(node->name);

    // Add a space to seperate the name from the children
    _string_buffer[_string_buffer_index++] = ' ';

    if (node->children == NULL)
    {
        return;
    }

    // When there is a file here instead of a dir, node->children will be NULL, and thus we shouldn't iterate over it.
    linked_list_node_t *iter = node->children == NULL ? NULL : node->children->head;
    while (iter != NULL)
    {
        preorder_traversal(iter->data);
        iter = iter->next;
    }

    // Add a delimiter to signify the end of the node children
    _string_buffer[_string_buffer_index++] = TREE_SERIALIZE_DELIMITER;
}

void tree_serialize_to_file(const file_tree_t *tree, const char* path)
{
    _string_buffer_index = 0;
    _string_buffer = (char *)malloc(SYSTEM_PATH_BUFFER_SIZE);

    preorder_traversal(tree->root);

    FILE *file = fopen(path, "w");
    fprintf(file, "%s", _string_buffer);
    fclose(file);
    free(_string_buffer);
}

