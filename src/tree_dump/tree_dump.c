#include "tree_dump.h"
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

    strcat(string_buffer, node->name);
    string_buffer_index += strlen(node->name);
    strcat(string_buffer, "\n");
    string_buffer_index += 1;

    assert(string_buffer_index < STRING_BUFFER_SIZE);

    // recurse on children
    linked_list_node_t *iter = node->children->head;
    while (iter != NULL)
    {
        file_node_t *child = (file_node_t *)iter->data;
        preorder_traversal(child);
        if (iter->next == NULL)
        {
            break;
        }
        iter = iter->next;
    }
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

// From our tree dump made with a preorder traversal, we can reconstruct the tree by reading the file line by line.
// file_tree_t *tree_load() {
//     file_tree_t *tree = file_tree_init();
//     FILE *file = fopen(TREE_DUMP_PATH, "r");
//     assert(file != NULL);

//     char line[MAX_FILE_NAME_LENGTH];
//     while (fgets(line, MAX_FILE_NAME_LENGTH, file) != NULL) {
//         line[strcspn(line, "\n")] = 0; // remove newline
//         file_tree_add_child(tree, tree->root, line);
//     }

//     fclose(file);
//     return tree;
// }