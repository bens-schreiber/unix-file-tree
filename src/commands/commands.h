#ifndef COMMANDS_H
#define COMMANDS_H

#include "../consts.h"
#include "../file_tree/file_tree.h"
#include "../linked_list/linked_list.h"

typedef char out_buffer_t[SYSTEM_PATH_BUFFER_SIZE];

extern const linked_list_t *system_path();

/// @brief Traverses the tree following the given path. Null if the path is invalid.
/// @param tree the file tree
/// @param path the path to traverse
/// @return the linked list of file nodes
/// @example "/a/b/c" will return a linked list with the nodes a, b, and c, asserting that the tree has the nodes a, b, and c
extern linked_list_t *unix_tree_traverse(const file_tree_t *tree, const char *path);

/// @brief Traverses the tree following the given path, and returns the closest valid path
/// @param tree the file tree
/// @param path the path to traverse
/// @return the linked list of file nodes
/// @example search for "/a/b/c" in a tree with only "/a/b" will return a linked list with the nodes a and b
extern linked_list_t *unix_tree_traverse_find_closest(const file_tree_t *tree, const char *path);

/// @brief Initializes the current path. This function should be called before any other command.
/// @param tree the file tree
extern void path_init(const file_tree_t *tree);

/// @brief Frees the current path
extern void path_free();

/// @brief Writes the present working directory to the out_buffer.
/// @param out_buffer the output of the command
extern void pwd(out_buffer_t out_buffer);

/// @brief Lists the contents of a directory
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the dir to list
extern void ls(out_buffer_t out_buffer, file_tree_t *tree, const char *path);

/// @brief Creates a file in the file tree
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the new dir
extern void mkdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path);

/// @brief Deletes a file from the file tree
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the file
extern void rmdir(out_buffer_t out_buffer, file_tree_t *tree, const char *path);

/// @brief Changes the current directory
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the new dir
extern void cd(out_buffer_t out_buffer, file_tree_t *tree, const char *dir_name);

/// @brief Creates a file (touch command, but wsu lab wanted it to be called create)
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the new file
extern void creat(out_buffer_t out_buffer, file_tree_t *tree, const char *path);

/// @brief Removes a file
/// @param out_buffer the output of the command
/// @param tree the file tree
/// @param path the path to the file
extern void rm(out_buffer_t out_buffer, file_tree_t *tree, const char *path);

/// @brief Loads the tree into a dump file on disk
/// @param tree the file tree
/// @param filename the name of the file to save to
extern void save(out_buffer_t out_buffer, file_tree_t *tree, const char *filename);

/// @brief Reloads the tree from a dump file on disk
/// @param tree the file tree
/// @param filename the name of the file to load from
/// @return the new tree
extern file_tree_t *reload(out_buffer_t out_buffer, file_tree_t *tree, const char *filename);

#endif // COMMANDS_H