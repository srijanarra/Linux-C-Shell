#include "headers.h"
#include "parse.h"
#include "function.h"

void parse()
{
    size_t size = 100;
    char *str = (char *) malloc(sizeof(char)*size);
    size_t n = getline(&str, &size, stdin);                     // to get input from terminal
    if(str[n-1] == '\n') {                                      // to remove newline character from the end of command
        str[n-1] = '\0';
        n--;
    }

    store_history(str);                                         // to store the command in history

    if(strcmp(str, "exit") == 0) {
        exit(0);
    }

    for(int j=0; j<1024;j++) {
        tok_commands[j] = (char *) malloc(1024);
    }

    for(int j=0; j<1024;j++) {
        tok_commands[j] = "\0";
    }

    char *p = strtok(str,";");
    int i=0;
    while(p)                                                     // tokenizing the entered commands with ';'
    {
        tok_commands[i] = p;
        p = strtok(NULL, ";");
        i++;
    }
    
    narg = 0;
    while(strcmp(tok_commands[narg],"\0") != 0) {
        narg++;
    }
}