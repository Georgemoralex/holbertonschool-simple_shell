#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

    pid_t process_id;
    pid_t p_process_id;

    process_id = getpid();
    p_process_id = getppid();

    printf("The process ID: %d\n", process_id);
    printf("The process ID of the parent function: %d\n", p_process_id);

    return 0;
}
