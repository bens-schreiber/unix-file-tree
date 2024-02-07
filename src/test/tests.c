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
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir1", 1);

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
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir", 1);

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        file_tree_add_child(tree, node, name, 0);
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
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir1", 1);

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
    file_node_t *node = file_tree_add_child(tree, tree->root, "dir", 1);

    file_node_t *dirs[MAX_FILE_CHILDREN];

    for (int i = 0; i < MAX_FILE_CHILDREN; i++)
    {
        char name[MAX_FILE_NAME_LENGTH];
        sprintf(name, "file%d", i);
        dirs[i] = file_tree_add_child(tree, node, name, 0);
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
    file_node_t *dir1 = file_tree_add_child(tree, tree->root, "dir1", 1);

    path_init(tree);

    linked_list_t *path = unix_tree_traverse(tree, "/dir1");
    assert(path != NULL);
    assert(path->size == 2);
    linked_list_free(path);

    path = unix_tree_traverse(tree, "dir1");
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
        file_tree_add_child(tree, tree->root, name, 0);
    }

    path_init(tree);
    out_buffer_t out_buffer;
    ls(out_buffer, tree, ".");

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
    file_node_t *dir1 = file_tree_add_child(tree, tree->root, "dir1", 1);
    file_node_t *dir2 = file_tree_add_child(tree, tree->root, "dir2", 1);

    path_init(tree);

    out_buffer_t out_buffer;

    // attempt up 1 dir from root, should stay at root
    cd(out_buffer, tree, "..");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    // switch to dir1, should be at /dir1/
    cd(out_buffer, tree, "dir1");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // up 1 dir to root
    cd(out_buffer, tree, "/");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    // switch to dir2, should be at /dir2/
    cd(out_buffer, tree, "dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir2/") == 0);

    // root from dir
    cd(out_buffer, tree, "/");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/") == 0);

    // ./ relative path
    cd(out_buffer, tree, "./dir1");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // dir2 isn't found
    cd(out_buffer, tree, "./dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir1/") == 0);

    // from dir1 to dir2
    cd(out_buffer, tree, "/dir2");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/dir2/") == 0);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_cd);
}

void test_command_mkdir()
{
    TEST(test_command_mkdir);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;

    mkdir(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir2");
    mkdir(out_buffer, tree, "dir3");

    ls(out_buffer, tree, ".");

    assert(tree->root->children_size == 3);
    assert(strstr(out_buffer, "dir1") != NULL);
    assert(strstr(out_buffer, "dir2") != NULL);
    assert(strstr(out_buffer, "dir3") != NULL);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_mkdir);
}

void test_command_rmdir()
{
    TEST(test_command_rmdir);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;

    mkdir(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir2");
    mkdir(out_buffer, tree, "dir3");

    rmdir(out_buffer, tree, "dir1");
    rmdir(out_buffer, tree, "dir2");
    rmdir(out_buffer, tree, "dir3");

    ls(out_buffer, tree, ".");

    assert(tree->root->children_size == 0);
    assert(strstr(out_buffer, "dir1") == NULL);
    assert(strstr(out_buffer, "dir2") == NULL);
    assert(strstr(out_buffer, "dir3") == NULL);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_rmdir);
}

void test_dir_crud()
{
    TEST(test_dir_crud);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;

    mkdir(out_buffer, tree, "a");
    cd(out_buffer, tree, "a");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/a/") == 0);

    mkdir(out_buffer, tree, "b");
    ls(out_buffer, tree, ".");
    assert(strstr(out_buffer, "b") != NULL);

    cd(out_buffer, tree, "b");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/a/b/") == 0);

    cd(out_buffer, tree, "..");
    rmdir(out_buffer, tree, "b");
    ls(out_buffer, tree, ".");
    assert(strstr(out_buffer, "b") == NULL);

    mkdir(out_buffer, tree, "c");
    cd(out_buffer, tree, "c");
    mkdir(out_buffer, tree, "d");
    cd(out_buffer, tree, "d");
    pwd(out_buffer);
    assert(strcmp(out_buffer, "/a/c/d/") == 0);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_dir_crud);
}

void test_command_creat() {
    TEST(test_command_creat);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;

    creat(out_buffer, tree, "file1");
    creat(out_buffer, tree, "file2");
    creat(out_buffer, tree, "file3");

    ls(out_buffer, tree, ".");

    assert(((file_node_t *)tree->root->children->head->data)->is_dir == 0);
    assert(tree->root->children_size == 3);
    assert(strstr(out_buffer, "file1") != NULL);
    assert(strstr(out_buffer, "file2") != NULL);
    assert(strstr(out_buffer, "file3") != NULL);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_creat);
}


void test_command_rm() {
    TEST(test_command_rm);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;

    creat(out_buffer, tree, "file1");
    creat(out_buffer, tree, "file2");
    creat(out_buffer, tree, "file3");

    rm(out_buffer, tree, "file1");
    rm(out_buffer, tree, "file2");
    rm(out_buffer, tree, "file3");

    ls(out_buffer, tree, ".");
    assert(tree->root->children_size == 0);
    assert(strstr(out_buffer, "file1") == NULL);
    assert(strstr(out_buffer, "file2") == NULL);
    assert(strstr(out_buffer, "file3") == NULL);

    path_free();
    tree = file_tree_free(tree);

    PASS(test_command_rm);
}

void test_tree_dump()
{
    TEST(test_tree_dump);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;
    mkdir(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir2");
    mkdir(out_buffer, tree, "dir3");

    cd(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir4");
    cd(out_buffer, tree, "../dir2");
    mkdir(out_buffer, tree, "dir5");
    cd(out_buffer, tree, "../dir3");
    mkdir(out_buffer, tree, "dir6");

    tree_dump(tree);

    // read the file
    FILE *file = fopen("../build/tree-dump.tree", "r");
    assert(file != NULL);

    char buffer[0xFFF];
    int size = fread(buffer, 1, 0xFFF, file);
    buffer[size] = '\0';

    assert(strcmp(buffer, "# #dir1 #dir4 $$#dir2 #dir5 $$#dir3 #dir6 $$$") == 0);

    fclose(file);
    tree = file_tree_free(tree);

    PASS(test_tree_dump);
}

void test_tree_load() {
    TEST(test_tree_load);

    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;
    mkdir(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir2");
    mkdir(out_buffer, tree, "dir3");

    cd(out_buffer, tree, "dir1");
    mkdir(out_buffer, tree, "dir4");
    cd(out_buffer, tree, "../dir2");
    mkdir(out_buffer, tree, "dir5");
    cd(out_buffer, tree, "../dir3");
    mkdir(out_buffer, tree, "dir6");

    tree_dump(tree);

    // read the file
    FILE *file = fopen("../build/tree-dump.tree", "r");
    assert(file != NULL);

    char buffer[0xFFF];
    int size = fread(buffer, 1, 0xFFF, file);
    buffer[size] = '\0';
    fclose(file);

    assert(strcmp(buffer, "# #dir1 #dir4 $$#dir2 #dir5 $$#dir3 #dir6 $$$") == 0);
    tree = file_tree_free(tree);

    // load the file
    tree = tree_load();

    // serialize it again
    tree_dump(tree);

    // read the file
    file = fopen("../build/tree-dump.tree", "r");
    assert(file != NULL);

    size = fread(buffer, 1, 0xFFF, file);
    buffer[size] = '\0';
    fclose(file);

    printf("%s\n", buffer);
    assert(strcmp(buffer, "# #dir1 #dir4 $$#dir2 #dir5 $$#dir3 #dir6 $$$") == 0);
    tree = file_tree_free(tree);


    PASS(test_tree_dump);
}