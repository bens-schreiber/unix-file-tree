#include "tree_serializer.h"
#include "../consts.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

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
        (void)linked_list_pop_head(preorder_trav_list);
        dfs_load(tree, node, preorder_trav_list);
    }
}

file_tree_t *tree_deserialize_from_file(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return NULL;
    }

    file_tree_t *tree = file_tree_init();
    char *buffer = (char *)malloc(SYSTEM_PATH_BUFFER_SIZE);
    fgets(buffer, SYSTEM_PATH_BUFFER_SIZE, file);
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
    (void)linked_list_pop_head(preorder_trav_list);
    dfs_load(tree, tree->root, preorder_trav_list);

    linked_list_free(preorder_trav_list);
    free(buffer);
    return tree;
}