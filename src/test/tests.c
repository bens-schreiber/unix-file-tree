#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../file-tree/file-tree.h"

#define TEST(name) printf("\nRunning test: " #name "\n");
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

void test_tree_insert()
{
    TEST(test_tree_insert);
    file_tree_t *tree = file_tree_init();
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir1");

    assert(tree->size == 2);
    assert(tree->root->children_size == 1);
    assert(tree->root->children[0] == node);
    assert(strcmp(node->name, "dir1") == 0);

    tree = file_tree_free(tree);
    assert(tree == NULL);

    PASS(test_tree_insert);
}

void test_tree_insert_multiple()
{
    TEST(test_tree_insert_multiple);

    file_tree_t *tree = file_tree_init();
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir");

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        file_tree_add_child(tree, node, name);
    }

    assert(tree->size == MAX_FILE_CHILDREN + 2);
    assert(node->children_size == MAX_FILE_CHILDREN);

    tree = file_tree_free(tree);
    assert(tree == NULL);

    PASS(test_tree_insert_multiple);
}