#ifndef COMMANDS_H
#define COMMANDS_H

#include "../consts.h"
#include "../file_tree/file_tree.h"
#include "../linked_list/linked_list.h"

typedef char out_buffer_t[SYSTEM_PATH_BUFFER_SIZE];

extern const linked_list_t *system_path();

extern linked_list_t *unix_tree_traverse(const file_tree_t *tree, const char *path);

/// @brief Initializes the current path. This function should be called before any other command
/// @param tree the file tree
extern void path_init(const file_tree_t *tree);

/// @brief Frees the current path
extern void path_free();

/// @brief Writes to the output buffer the current path
/// @param out_buffer the output of the command
extern void pwd(out_buffer_t out_buffer);

/// @brief Lists the contents of a directory
/// @param out_buffer the output of the command
extern void ls(out_buffer_t out_buffer);

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

#endif // COMMANDS_H