#include "sshell.h"

int main(int argc, char **argv)
{
    char prompt[] = "(hsh) ";
    Bool status = true;

    (void)argc;
    name = (argv[0] != NULL) ? argv[0] : NULL;

    while (status)
    {
        char *input = get_user_input();
        write(1, prompt, strlen(prompt));
        free(input);
    }

    return (0);
}
