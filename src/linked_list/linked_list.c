#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    node->data = data;
    assert(node != NULL);
    assert(node->data != NULL);

    node->next = list->head;
    list->head = node;
    if (list->tail == NULL)
    {
        list->tail = node;
    }

    list->size++;
}

void linked_list_insert_tail(linked_list_t *list, const void *data)
{
    assert(list != NULL);

    linked_list_node_t *node = malloc(sizeof(linked_list_node_t));
    node->data = data;
    assert(node != NULL);
    assert(node->data != NULL);

    node->next = NULL;
    if (list->tail != NULL)
    {
        list->tail->next = node;
    }
    list->tail = node;
    if (list->head == NULL)
    {
        list->head = node;
    }

    list->size++;
}

unsigned char linked_list_delete(linked_list_t *list, const void *data)
{
    assert(list != NULL);
    assert(data != NULL);

    linked_list_node_t *prev = NULL;
    linked_list_node_t *curr = list->head;

    while (curr != NULL)
    {
        if (curr->data == data)
        {
            if (prev != NULL)
            {
                prev->next = curr->next;
            }
            if (curr == list->head)
            {
                list->head = curr->next;
            }
            if (curr == list->tail)
            {
                list->tail = prev;
            }
            free(curr);
            list->size--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

void linked_list_pop_head(linked_list_t *list)
{
    if (list != NULL && list->head != NULL)
    {
        linked_list_node_t *next = list->head->next;
        free(list->head);
        list->head = next;
        if (list->head == NULL)
        {
            list->tail = NULL;
        }
        list->size--;
    }
}

void linked_list_pop_tail(linked_list_t *list)
{
    if (list != NULL && list->tail != NULL)
    {
        linked_list_node_t *curr = list->head;
        if (curr == list->tail)
        {
            free(curr);
            list->head = NULL;
            list->tail = NULL;
            list->size--;
            return;
        }
        while (curr->next != list->tail)
        {
            curr = curr->next;
        }
        free(list->tail);
        list->tail = curr;
        curr->next = NULL;
        list->size--;
    }
}

void linked_list_free(linked_list_t *list)
{
    if (list == NULL)
    {
        return;
    }

    linked_list_node_t *curr = list->head;
    while (curr != NULL)
    {
        linked_list_node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->tail = NULL;
    free(list);
}
