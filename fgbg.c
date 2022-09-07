#include "headers.h"
#include "function.h"
#include "parse.h"
#include "prompt.h"

void sys_commands(char *strcmds[], int n, int bg) 
{
    char *arrptr[n+1];
    for(int i=0; i<n; i++) {
        arrptr[i] = (char *) malloc(512);
        arrptr[i] = strcmds[i];
    }
    arrptr[n] = NULL;

    int ret_pid = fork();
    if(ret_pid < 0) {
        perror("Error in sys_commands() while calling fork(): ");
        return;
    }
    else if(ret_pid == 0) {
        if(execvp(arrptr[0], arrptr) == -1) {
            perror("Invalid command");
            exit(40);
        }
        exit(0);
    }
    else {
        if(bg == 1) {
            printf("%d\n", ret_pid);
            add_process(arrptr[0], ret_pid);
        }
        else {
            int status;
            waitpid(ret_pid, &status, 0);
        }
    }
}