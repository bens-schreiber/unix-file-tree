#include <stdlib.h>
#include <string.h>
#include "commands.h"

static linked_list_t *_system_path = NULL;

void path_init(const file_tree_t *tree) {
    _system_path = linked_list_init();
    linked_list_insert(_system_path, tree->root);
}

void path_free() {
    linked_list_free(_system_path);
}

void pwd(out_buffer_t out_buffer) {
    linked_list_node_t *iter = _system_path->head;
    int index = 0;
    while (iter != NULL) {
        file_node_t *node = (file_node_t *)iter->data;
        out_buffer[index] = '/';
        index += 1;
        for (int i = 0; i < strlen(node->name); i++) {
            out_buffer[index] = node->name[i];
            index += 1;
        }
        iter = iter->next;
    }
    out_buffer[index] = '\0';
}