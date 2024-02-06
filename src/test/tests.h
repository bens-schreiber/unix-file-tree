#ifndef TESTS_H
#define TESTS_H

/// @brief Initialize a file tree, inspect initial attributes, free it from memory
extern void test_tree_init();

/// @brief Insert a file node into a file tree
extern void test_tree_insert();

/// @brief Insert many file nodes into the file tree
extern void test_tree_insert_multiple();

/// @brief Delete a file node from the file tree
extern void test_tree_delete();

/// @brief Delete many file nodes from the file tree
extern void test_tree_delete_multiple();

/// @brief present working directory command
extern void test_command_pwd();

/// @brief list command
extern void test_command_ls();

/// @brief search for a file inside of the file tree
extern void test_path_search();

/// @brief change directory command
extern void test_command_cd();

/// @brief make a directory command
extern void test_command_mkdir();

/// @brief remove a file command
extern void test_command_rmdir();

/// @brief create, read, update, delete directory. More extensive than test_command_mkdir and test_command_rmdir
extern void test_dir_crud();

/// @brief create a file command
extern void test_command_creat();

/// @brief remove a file command
extern void test_command_rm();

/// @brief Dump the file tree to the tree-dump file
extern void test_tree_dump();

/// @brief load a file tree from the tree-dump file
extern void test_tree_load();

/// @brief Run all tests. Done in a linear fashion such that prequisites tests are ran first.
static inline void test()
{
    test_tree_init();
    test_tree_insert();
    test_tree_insert_multiple();
    test_tree_delete();
    test_tree_delete_multiple();
    test_command_pwd();
    test_path_search();
    test_command_ls();
    test_command_cd();
    test_command_mkdir();
    test_command_rmdir();
    test_dir_crud();
    test_command_creat();
    test_command_rm();
    test_tree_dump();
    test_tree_load();
}

#endif // TESTS_H