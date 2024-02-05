#include "linked-list.h"
#include <stdlib.h>

linked_list_t *linked_list_init()
{
    linked_list_t *list = malloc(sizeof(linked_list_t));
    assert(list != NULL);

    list->head = NULL;
    return list;
}

void linked_list_insert(linked_list_t *list, void *data)
{
    linked_list_node_t *node = malloc(sizeof(linked_list_node_t));
    assert(node != NULL);

    node->data = data;
    node->next = list->head;
    list->head = node;
}

void linked_list_delete(linked_list_t *list, void *data)
{
    _linked_list_delete_recursive(list->head, data);
}

void _linked_list_delete_recursive(linked_list_node_t *node, void *data)
{
    if (node == NULL)
    {
        return;
    }

    if (node->data == data)
    {
        linked_list_node_t *next = node->next;
        free(node);
        node = next;
        return;
    }

    _linked_list_delete_recursive(node->next, data);
}

void linked_list_free(linked_list_t *list)
{
    linked_list_node_t *curr = list->head;
    while (curr != NULL)
    {
        linked_list_node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}