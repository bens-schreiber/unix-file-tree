#include "tests.h"
#include "../consts.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "../file_tree/file_tree.h"
#include "../tree_dump/tree_dump.h"
#include "../commands/commands.h"

#define TEST(name) printf("\nRunning test: " #name "\n");
#define PASS(name) printf("Test passed: " #name "\n");

#define MAX_FILE_CHILDREN 0xFF

void test_tree_init()
{
    TEST(test_tree_init);

    file_tree_t *tree = file_tree_init();

    assert(strcmp(tree->root->name, TREE_ROOT_NAME) == 0);
    assert(tree->size == 1);
    assert(tree->root->children_size == 0);
    assert(tree->root->children->head == NULL);

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
    assert(tree->root->children->head->data == node);
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

void test_tree_delete()
{
    TEST(test_tree_delete);

    file_tree_t *tree = file_tree_init();
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir1");

    node = file_tree_delete_child(tree, tree->root, node);
    assert(tree->size == 1);
    assert(tree->root->children_size == 0);
    assert(tree->root->children->head == NULL);

    tree = file_tree_free(tree);
    assert(tree == NULL);

    PASS(test_tree_delete);
}

void test_tree_delete_multiple()
{
    TEST(test_tree_delete_multiple);

    file_tree_t *tree = file_tree_init();
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir");

    file_node_t *dirs[MAX_FILE_CHILDREN];

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        dirs[i] = file_tree_add_child(tree, node, name);
    }

    // Delete half of them
    for (int i = 0; i < MAX_FILE_CHILDREN / 2; i++)
    {
        file_tree_delete_child(tree, node, dirs[i]);
    }

    // Delete all of them
    file_tree_delete_child(tree, tree->root, node);

    assert(tree->size == 1);
    assert(tree->root->children_size == 0);
    assert(tree->root->children->head == NULL);

    tree = file_tree_free(tree);

    PASS(test_tree_delete_multiple);
}

void test_tree_dump()
{
    TEST(test_tree_dump);

    file_tree_t *tree = file_tree_init();
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir");

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        file_tree_add_child(tree, node, name);
    }

    tree_dump(tree);

    tree = file_tree_free(tree);

    PASS(test_tree_dump);
}

void test_command_pwd()
{
    TEST(test_command_pwd);

    file_tree_t *tree = file_tree_init();
    path_init(tree);
    out_buffer_t out_buffer;
    pwd(out_buffer);

    printf("out_buffer: %s\n", out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_pwd);
}