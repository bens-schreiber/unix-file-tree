#include "test/tests.h"
#include "terminal/terminal.h"

// #define DEBUG

int main(int argc, char const *argv[])
{
#ifdef DEBUG
    test();
#else
    terminal_loop();
#endif
    return 0;
}
