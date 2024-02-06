#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list_node {
    const void *data;
    struct linked_list_node *next;
} linked_list_node_t;

typedef struct {
    linked_list_node_t *head;
    linked_list_node_t *tail;
    unsigned int size;
} linked_list_t;

/// @brief Initializes a new linked list, with head and tail set to NULL
extern linked_list_t *linked_list_init();

/// @brief Inserts data at the front of the list
/// @param list the list to insert into
/// @param data the data to insert
extern void linked_list_insert(linked_list_t *list, const void *data);

/// @brief Inserts data at the back of the list
/// @param list the list to insert into
/// @param data the data to insert
extern void linked_list_insert_tail(linked_list_t *list, const void *data);

/// @brief Finds the first instance of data in the list and deletes it
/// @param list the list to delete from
/// @param data the data to delete
/// @return 1 if the data was found and deleted, 0 otherwise
extern unsigned char linked_list_delete(linked_list_t *list, const void *data);

/// @brief Pops the front of the list.
/// @param list 
extern void linked_list_pop_head(linked_list_t *list);

/// @brief Pops the end of the list.
/// @param list 
extern void linked_list_pop_tail(linked_list_t *list);

/// @brief Frees the list and all of its nodes. Does not free the data.
/// @param list the list to free
extern void linked_list_free(linked_list_t *list);

#endif // LINKED_LIST_H

