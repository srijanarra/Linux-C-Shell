#include "headers.h"
#include "function.h"

void store_history(char *command)
{
    FILE *fd = fopen(HISTORY, "r");
    if(fd == NULL) {
        fd = fopen(HISTORY, "a");
        // perror("Error in fopen: ");
        // return;
        if(fd == NULL) {
            perror("Error creating file");
            return;
        }
    }
    // lseek(fileno(fd), 0, SEEK_SET);
    char *arr_commands[MAX_HISTORY-1];
    int num_lines = 0;
    for(int i=0; i<MAX_HISTORY-1; i++) {
        arr_commands[i] = (char *) malloc(1024);
    }

    for(int i=0; i<MAX_HISTORY-1; i++) {
        if(fgets(arr_commands[i], 1000, fd) == NULL) {
            break;
        }
        arr_commands[i][strcspn(arr_commands[i], "\n")] = 0;
        // printf("2\n");
        num_lines++;
        
    }
    fclose(fd);

    // printf("numlines: %d\n", num_lines);


    fd = fopen(HISTORY, "w");
    if(fd == NULL) {
        perror("Error in fopen: ");
        return;
    }
    if(strcmp(arr_commands[0], command) != 0) {
        fputs(command, fd);
        if(num_lines > 0) {
            fputs("\n", fd);
        }
    }
    for(int i=0; i<num_lines; i++) {
        if((arr_commands[i] == NULL)) break;
        // printf("%s\n", arr_commands[i]);
       
        fputs(arr_commands[i], fd);
        if(i != (num_lines - 1)) {
            fputs("\n", fd);
        }
    }



    // if(fgets(buf, 1000, fd) == NULL) {
    //     // write(fd, command, 1000);
    //     fflush(fd);
    //     fprintf(fd, "%s", command);
    //     return;
    // }

    // char *ret;

    // char *buf1 = (char *) malloc(1024);
    // char *buf2 = (char *) malloc(1024);
    // buf1 = command;
    // for(int i=0; i<MAX_HISTORY; i++) {
    //     ret = fgets(buf2, 1000, fd);
    //     // write(fd, buf1, 1000);
    //     fflush(fd);
    //     fprintf(fd, "%s", buf1);
    //     if(ret == NULL) {
    //         return;
    //     }
    //     strcpy(buf1, buf2);
    // }

    // free(buf1);
    // free(buf2);
    for(int i=0; i<MAX_HISTORY-1; i++) {
        free(arr_commands[i]);
    }
    fclose(fd);

}

void print_history(char *his_args[], int n)
{
    if(n > 2) {
        printf("Too many arguments for history!");
        return;
    }

    int num_commands = 10;

    if(n == 2) {
        num_commands = atoi(his_args[1]);
    }

    char *buf = (char *) malloc(1024);
    FILE *fd = fopen(HISTORY, "r");
    if(fd == NULL) {
        perror("Error in fopen: ");
        return;
    }
    for(int i=0; i<num_commands; i++) {
        if(fgets(buf, 1000, fd) == NULL) {
            break;
        }

        printf("%s", buf);
    }
    printf("\n");

    free(buf);
    fclose(fd);
}