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

/// @brief Dump the file tree to the tree-dump file
extern void test_tree_dump();

/// @brief present working directory command
extern void test_command_pwd();

/// @brief list command
extern void test_command_ls();

/// @brief search for a file inside of the file tree
extern void test_path_search();

/// @brief change directory command
extern void test_command_cd();

/// @brief Run all tests. Done in a linear fashion such that prequisites tests are ran first.
static inline void test()
{
    test_tree_init();
    test_tree_insert();
    test_tree_insert_multiple();
    test_tree_delete();
    test_tree_delete_multiple();
    test_tree_dump();
    test_command_pwd();
    test_path_search();
    test_command_ls();
    test_command_cd();
}

#endif // TESTS_H