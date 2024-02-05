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

/// @brief Run all tests.
static inline void test()
{
    test_tree_init();
    test_tree_insert();
    test_tree_insert_multiple();
    test_tree_delete();
}

#endif // TESTS_H