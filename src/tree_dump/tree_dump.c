#include "tree_dump.h"
#include "../consts.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define STRING_BUFFER_SIZE 0xFFF

static char *string_buffer;
static int string_buffer_index;

void preorder_traversal(const file_node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    string_buffer[string_buffer_index++] = node->is_dir ? TREE_SERIALIZE_DIR_DELIMITER : TREE_SERIALIZE_FILE_DELIMITER;
    strcpy(string_buffer + string_buffer_index, node->name);
    string_buffer_index += strlen(node->name);
    string_buffer[string_buffer_index++] = ' ';

    if (node->children == NULL)
    {
        return;
    }
    linked_list_node_t *iter = node->children == NULL ? NULL : node->children->head;
    while (iter != NULL)
    {
        preorder_traversal(iter->data);
        iter = iter->next;
    }

    string_buffer[string_buffer_index++] = TREE_SERIALIZE_DELIMITER;
}

void tree_dump(const file_tree_t *tree)
{
    string_buffer_index = 0;
    string_buffer = (char *)malloc(STRING_BUFFER_SIZE);
    preorder_traversal(tree->root);

    FILE *file = fopen(TREE_DUMP_PATH, "w");
    assert(file != NULL);
    fprintf(file, "%s", string_buffer);
    fclose(file);
    free(string_buffer);
}

void dfs_load(file_tree_t *tree, file_node_t *parent, linked_list_t *preorder_trav_list)
{
    assert(preorder_trav_list != NULL);

    while (preorder_trav_list->size > 0)
    {
        char *phrase = preorder_trav_list->head->data;
        if (phrase[0] == TREE_SERIALIZE_DELIMITER)
        {
            preorder_trav_list->head->data++;
            return;
        }
        if (phrase[0] == '\0')
        {
            return;
        }

        unsigned char is_dir = phrase[0] == TREE_SERIALIZE_DIR_DELIMITER;
        phrase++;

        file_node_t *node = file_tree_add_child(tree, parent, phrase, is_dir);
        linked_list_pop_head(preorder_trav_list);
        dfs_load(tree, node, preorder_trav_list);
    }
}

file_tree_t *tree_load()
{
    FILE *file = fopen(TREE_DUMP_PATH, "r");
    if (file == NULL)
    {
        return NULL;
    }

    file_tree_t *tree = file_tree_init();
    char *buffer = (char *)malloc(STRING_BUFFER_SIZE);
    fgets(buffer, STRING_BUFFER_SIZE, file);
    fclose(file);

    linked_list_t *preorder_trav_list = linked_list_init();

    // load buffer into linked list seperated by spaces
    // this could be done more efficiently but c strings are the devil
    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        char *token_copy = strdup(token);
        linked_list_insert_tail(preorder_trav_list, token_copy);
        token = strtok(NULL, " ");
    }

    // extra pop to remove the first delimiter
    linked_list_pop_head(preorder_trav_list);
    dfs_load(tree, tree->root, preorder_trav_list);

    free(buffer);
    return tree;
}