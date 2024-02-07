#include "terminal.h"
#include "../file_tree/file_tree.h"
#include "../commands/commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// @brief The tree that the terminal is currently operating on.
static file_tree_t *tree = NULL;

#define COMMAND(name) void process_##name(out_buffer_t out_buffer, const char *arg)
COMMAND(ls) { ls(out_buffer, tree, arg); }
COMMAND(cd) { cd(out_buffer, tree, arg); }
COMMAND(pwd) { pwd(out_buffer); }
COMMAND(mkdir) { mkdir(out_buffer, tree, arg); }
COMMAND(rmdir) { rmdir(out_buffer, tree, arg); }
COMMAND(exit) { exit(0); }
COMMAND(quit) { exit(0); }
COMMAND(creat) { creat(out_buffer, tree, arg); }
COMMAND(rm) { rm(out_buffer, tree, arg); }
COMMAND(save) { save(out_buffer, tree, arg); }
COMMAND(reload) { tree = reload(out_buffer, tree, arg); }
#undef COMMAND

/// @brief A mapping of command names to their respective functions. Better than if-else chains.
struct command_map
{
    char *command;
    void (*function)(out_buffer_t, const char *);
};

/// @brief The list of commands that the terminal can process.
struct command_map commands[] = {
    {"ls", process_ls},
    {"cd", process_cd},
    {"pwd", process_pwd},
    {"mkdir", process_mkdir},
    {"rmdir", process_rmdir},
    {"exit", process_exit},
    {"creat", process_creat},
    {"rm", process_rm},
    {"quit", process_quit},
    {"save", process_save},
    {"reload", process_reload}};

/// @brief Trims the command of any trailing newline or carriage return characters.
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

/// @brief Processes the given command and argument, and writes the result to the out_buffer.
void process_command(char *command, out_buffer_t out_buffer, const char *arg)
{
    trim_command(command);
    trim_command(arg);
    for (int i = 0; i < sizeof(commands) / sizeof(struct command_map); i++)
    {
        if (strcmp(command, commands[i].command) == 0)
        {

            commands[i].function(out_buffer, arg);
            return;
        }
    }
    printf("err: command not found: %s\n", command);
}

typedef char in_buffer_t[0xFFF];

void terminal_loop()
{

    // Initialize the file tree and the path
    tree = file_tree_init();
    path_init(tree);

    // All output from any command will be written to this buffer
    out_buffer_t out_buffer;

    // All input from the user will be written to this buffer
    in_buffer_t in_buffer;

    char *command = NULL;
    while (1)
    {

        // Show pwd on terminal loop
        pwd(out_buffer);
        printf("%s $ ", out_buffer);
        fgets(in_buffer, 0xFFF, stdin);

        // Clear the output buffer
        memset(out_buffer, 0, sizeof(out_buffer));

        // Tokenize the input buffer, we're looking for {command} {arg}
        char *command = strtok(in_buffer, " ");

        // If the user just pressed enter, ignore it
        if (strcmp(command, "\n") == 0 || strcmp(command, "\r") == 0 || command == NULL)
        {
            continue;
        }

        // Arg can be null
        const char *arg = strtok(NULL, " ");

        process_command(command, out_buffer, arg);

        // Print the output buffer if it has been written to
        if (out_buffer[0] != '\0')
        {
            printf("%s", out_buffer);
        }
    }
}