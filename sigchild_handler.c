#include "headers.h"
#include "function.h"
#include "parse.h"
#include "prompt.h"

void sigchild_handler(int n)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);

    if(pid < 0) {
        return;
    }
    else if(pid == 0) {
        return;
    }

    char *comm_name = (char *)malloc(512);
    char *path = (char *)malloc(512);
    char *buf = (char *)malloc(1024 * sizeof(char));

    sprintf(path, "/proc/%d/status", pid);
    comm_name = search_process(pid);


    if(WIFEXITED(status)) {
        if(WEXITSTATUS(status) != 40) {
            printf("\n%s with pid %d exited normally\n", comm_name, pid);
        }
    }
    else {
        printf("\n%s with pid %d exited abnormally\n", comm_name, pid);
    }

    prompt();
    fflush(stdout);
}