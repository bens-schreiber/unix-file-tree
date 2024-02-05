#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../file-tree/file-tree.h"

#define TEST(name) printf("\n\nRunning test: " #name "\n");
#define PASS(name) printf("Test passed: " #name "\n");

void test_tree_init()
{
    TEST(test_tree_init);
    file_tree_t *tree = file_tree_init();

    assert(strcmp(tree->root->name, "/") == 0);
    assert(tree->size == 1);
    assert(tree->root->children_size == 0);
    assert(tree->root->children[0] == NULL);

    tree = file_tree_free(tree);
    assert(tree == NULL);

    PASS(test_tree_init);
}