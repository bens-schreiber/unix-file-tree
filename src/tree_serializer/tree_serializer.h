#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "../file_tree/file_tree.h"

/// @brief Takes a file tree and serializes it into a string via preorder traversal.
/// @param tree The file the serialized tree should dump into.
extern void tree_serialize_to_file(const file_tree_t *tree, const char *path);

/// @brief From the given path, attempt to load the file system tree. Assumes the file is in the correct format.
/// @param path The path to the file to load from.
extern file_tree_t *tree_deserialize_from_file(const char *path);

#endif // TREE_DUMP_H