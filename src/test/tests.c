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

    assert(strcmp(out_buffer, "/") == 0);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_pwd);
}

void test_path_search()
{
    TEST(test_path_search);

    file_tree_t *tree = file_tree_init();
    file_node_t *dir1 = file_tree_add_child(tree, tree->root, "dir1");
    file_node_t *dir2 = file_tree_add_child(tree, tree->root, "dir2");

    path_init(tree);

    linked_list_t *path = tree_search_with_path(tree->root, "/dir1");
    assert(path != NULL);
    assert(path->size == 2);
    linked_list_free(path);

    path = tree_search_with_path(tree->root, "/dir2");
    assert(path != NULL);
    assert(path->size == 2);
    linked_list_free(path);

    tree = file_tree_free(tree);

    PASS(test_path_search);
}

void test_command_ls()
{
    TEST(test_command_ls);

    file_tree_t *tree = file_tree_init();

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        file_tree_add_child(tree, tree->root, name);
    }

    path_init(tree);
    out_buffer_t out_buffer;
    ls(out_buffer);

    // We expect there to be MAX_FILE_CHILDREN files in the dir
    // Count the number of lines
    int count = 0;
    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        if (strstr(out_buffer, name) != NULL)
        {
            count++;
        }
    }

    assert(count == MAX_FILE_CHILDREN);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_ls);
}

void test_command_cd()
{
    TEST(test_command_cd);

    file_tree_t *tree = file_tree_init();
    file_node_t *dir1 = file_tree_add_child(tree, tree->root, "dir1");
    file_node_t *dir2 = file_tree_add_child(tree, tree->root, "dir2");

    path_init(tree);

    out_buffer_t out_buffer;

    // to dir1
    cd(tree, "dir1");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // up 1 dir
    cd(tree, "..");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    // root to dir
    cd(tree, "dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir2/") == 0);

    // root from dir
    cd(tree, "/");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    // ./ relative path
    cd(tree, "./dir1");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // dir2 isn't found
    cd(tree, "./dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // from dir1 to dir2
    cd(tree, "/dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir2/") == 0);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_cd);
}

void test_command_mkdir() {
    TEST(test_command_mkdir);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    mkdir(tree, "dir1");
    mkdir(tree, "dir2");
    mkdir(tree, "dir3");

    out_buffer_t out_buffer;
    ls(out_buffer);

    assert(tree->root->children_size == 3);
    assert(strstr(out_buffer, "dir1") != NULL);
    assert(strstr(out_buffer, "dir2") != NULL);
    assert(strstr(out_buffer, "dir3") != NULL);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_mkdir);
}