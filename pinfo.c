#include "headers.h"
#include "function.h"
#include "prompt.h"
#include "parse.h"

void pinfo(char *pinfo_cmds[], int n) 
{
    if(n > 2) {
        printf("Too many arguments for pinfo!");
        return;
    }

    int pid;
    char *path = (char *)malloc(512);
    char state;
    char *buf = (char *)malloc(1024 * sizeof(char));
    int tgid, ppid;

    if(n == 1) {
        pid = getpid();
    }
    else {
        pid = atoi(pinfo_cmds[1]);
    }

    printf("pid : %d\n", pid);

    sprintf(path, "/proc/%d/status", pid);

    FILE* fd = fopen(path, "r");
    if(fd == NULL) {
        perror("Error in fopen: ");
    }
    fgets(buf, 512, fd);
    fgets(buf, 512, fd);
    sscanf(buf, "State:\t%c", &state);
    fgets(buf, 512, fd);
    sscanf(buf, "Tgid:\t%d", &tgid);
    fgets(buf, 512, fd);
    fgets(buf, 512, fd);
    sscanf(buf, "PPid:\t%d", &ppid);
    if(tgid == ppid) {
        printf("process status : {%c+}\n", state);    
    }
    else {
        printf("process status : {%c}\n", state);
    }
    fclose(fd);

    sprintf(path, "/proc/%d/statm", pid);
    fd = fopen(path, "r");
    fgets(buf, 512, fd);
    char *first_str = strtok(buf, " \t");
    printf("memory : %s {Virtual Memory}\n", first_str);
    fclose(fd);

    sprintf(path, "/proc/%d/exe", pid);
    int len = readlink(path, buf, 100);
    
    if(len == -1){
        perror("Error in readlink: ");
        return;
    }
    buf[len] = '\0';

    printf("executable path : %s\n", buf);

    free(path);
    free(buf);
}