#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_listnode {
    void *data;
    struct node *next;
} linked_list_node_t;

typedef struct linked_list {
    linked_list_node_t *head;
    linked_list_node_t *tail;
} linked_list_t;

extern linked_list_t *linked_list_init();

extern void linked_list_insert(linked_list_t *list, void *data);

extern void linked_list_delete(linked_list_t *list, void *data);

extern void linked_list_free(linked_list_t *list);

#endif // LINKED_LIST_H

