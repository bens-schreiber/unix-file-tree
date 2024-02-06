#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

linked_list_t *linked_list_init()
{
    linked_list_t *list = malloc(sizeof(linked_list_t));
    assert(list != NULL);

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void linked_list_insert(linked_list_t *list, const void *data)
{
    assert(list != NULL);

    linked_list_node_t *node = malloc(sizeof(linked_list_node_t));
    assert(node != NULL);

    node->data = data;
    node->next = list->head;
    list->head = node;
    if (list->tail == NULL)
    {
        list->tail = node;
    }

    list->size++;
}

unsigned char linked_list_delete(linked_list_t *list, const void *data)
{
    assert(list != NULL);
    assert(list->size > 0);
    linked_list_node_t *prev = NULL;
    linked_list_node_t *curr = list->head;
    while (curr != NULL)
    {
        if (curr->data == data)
        {
            if (prev == NULL)
            {
                list->head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    list->size--;
    return 0;
}

void linked_list_free(linked_list_t *list)
{
    assert(list != NULL);

    linked_list_node_t *curr = list->head;
    while (curr != NULL)
    {
        linked_list_node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}