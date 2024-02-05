#ifndef TESTS_H
#define TESTS_H

/// @brief Initialize a file tree, inspect initial attributes, free it from memory
extern void test_tree_init();

/// @brief Run all tests.
static inline void test()
{
    test_tree_init();
}

#endif // TESTS_H