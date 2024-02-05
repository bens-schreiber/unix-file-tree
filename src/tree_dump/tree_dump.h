#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "../file_tree/file_tree.h"

#define TREE_DUMP_PATH "tree-dump.tree"

/// @brief Dumps a file tree into a string, and puts it into the tree-dump file. Via an O(n) post-order traversal.
/// @param tree The file tree to dump.
extern void tree_dump(const file_tree_t *tree);

/// @brief Loads a file tree from a tree-dump file.
extern file_tree_t *tree_load();

#endif // TREE_DUMP_H