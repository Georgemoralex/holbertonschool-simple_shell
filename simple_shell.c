#include "sshell.h"

int main(int argc, char **argv)
{
    char prompt[] = "(hsh) ";
    Bool status = true;

    (void)argc;
    name = (argv[0] != NULL) ? argv[0] : NULL;

    while (status)
    {
        write(1, prompt, strlen(prompt));
        char *input = get_user_input();

        if (input != NULL && *input != '\0') {
            execute_command(input);
        }

        free(input);
    }

    return (0);
}
