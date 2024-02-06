#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "../linked_list/linked_list.h"
#include "../consts.h"

typedef const char file_name_t[MAX_FILE_NAME_LENGTH];

typedef struct file_node
{
    char name[MAX_FILE_NAME_LENGTH];
    unsigned int children_size;
    unsigned char is_dir;
    linked_list_t *children;
} file_node_t;

/// @brief Allocates a file node on the heap with a name, with no children (NULL)
/// @param name the name of the file node
extern file_node_t *file_node_init(file_name_t name, unsigned char is_dir);

/// @brief Adds a child to the file node
/// @param name The name that child should have
/// @param node The parent node
/// @return The new child
extern file_node_t *file_node_add_child(file_node_t *node, file_name_t name, unsigned char is_dir);

/// @brief Deletes a file node and all of its children
/// @param node the file node
/// @return NULL
extern void *file_node_free(file_node_t *node);

typedef struct file_tree
{
    file_node_t *root;
    unsigned int size;
} file_tree_t;

/// @brief Allocates a file tree on the heap with a root node and size 1
/// @return The file tree
extern file_tree_t *file_tree_init();

/// @brief Deletes a file tree and all of its nodes
extern void *file_tree_free(file_tree_t *tree);

/// @brief Traverses the file tree and frees all nodes
/// @param tree the file tree
extern void file_tree_out(const file_tree_t *tree);

/// @brief Adds a new child to node
/// @param tree the file tree
/// @param node the parent node
/// @param name name of the new file
/// @return the child node
static inline file_node_t *file_tree_add_child(file_tree_t *tree, file_node_t *node, file_name_t name, unsigned char is_dir)
{
    file_node_t *child = file_node_add_child(node, name, is_dir);
    tree->size++;
    return child;
}

/// @brief Deletes a child from a parent node. The parent is not freed.
/// @param tree the file tree
/// @param parent the parent node
/// @param node the node to delete from the parent
/// @return NULL
void *file_tree_delete_child(file_tree_t *tree, file_node_t *parent, file_node_t *node);

#endif // FILE_TREE_H
