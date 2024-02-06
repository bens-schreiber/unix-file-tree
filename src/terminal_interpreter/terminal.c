#include "terminal.h"
#include "../file_tree/file_tree.h"
#include "../commands/commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND(name) void process_##name(file_tree_t *tree, out_buffer_t out_buffer, const char *arg)

COMMAND(ls) { ls(out_buffer, tree, arg); }
COMMAND(cd) { cd(out_buffer, tree, arg); }
COMMAND(pwd) { pwd(out_buffer); }
COMMAND(mkdir) { mkdir(out_buffer, tree, arg); }
COMMAND(rmdir) { rmdir(out_buffer, tree, arg); }
COMMAND(exit) { exit(0); }

typedef char in_buffer_t[0xFFF];

struct command_map
{
    char *command;
    void (*function)(file_tree_t *, out_buffer_t, const char *);
};

struct command_map commands[] = {
    {"ls", process_ls},
    {"cd", process_cd},
    {"pwd", process_pwd},
    {"mkdir", process_mkdir},
    {"rmdir", process_rmdir},
    {"exit", process_exit},
};

void trim_command(const char *command)
{
    if (command == NULL)
    {
        return;
    }
    char *p = strchr(command, '\n');
    if (p)
        *p = 0;

    p = strchr(command, '\r');
    if (p)
        *p = 0;
}

void process_command(file_tree_t *tree, char *command, out_buffer_t out_buffer, const char *arg)
{
    trim_command(command);
    trim_command(arg);
    for (int i = 0; i < sizeof(commands) / sizeof(struct command_map); i++)
    {
        if (strcmp(command, commands[i].command) == 0)
        {
            commands[i].function(tree, out_buffer, arg);
            return;
        }
    }
    printf("err: command not found: %s\n", command);
}

void terminal_loop()
{
    file_tree_t *tree = file_tree_init();
    path_init(tree);

    out_buffer_t out_buffer;
    in_buffer_t in_buffer;

    char *command = NULL;

    while (1)
    {
        pwd(out_buffer);
        printf("%s $ ", out_buffer);
        fgets(in_buffer, 0xFFF, stdin);

        memset(out_buffer, 0, sizeof(out_buffer));

        char *command = strtok(in_buffer, " ");

        if (strcmp(command, "\n") == 0 || strcmp(command, "\r") == 0 || command == NULL)
        {
            continue;
        }

        const char *arg = strtok(NULL, " ");

        process_command(tree, command, out_buffer, arg);

        if (out_buffer[0] != '\0')
        {
            printf("%s", out_buffer);
        }
    }
}