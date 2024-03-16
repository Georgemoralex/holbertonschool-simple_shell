#include "minishell.h"

int main(int argc, char **argv)
{
    char *cmdline;
    char prompt[] = "(hsh) ";
    Bool status = true;

    (void)argc;

    name = (argv[0] != NULL) ? argv[0] : NULL;

    while (status)
    {
        write(1, prompt, strlen(prompt));
        cmdline = get_user_input();
        free(cmdline);
    }

    return 0;
}

