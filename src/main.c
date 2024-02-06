#include "test/tests.h"
#include "terminal_interpreter/terminal.h"

int main(int argc, char const *argv[])
{
    test();
    terminal_loop();
    return 0;
}
