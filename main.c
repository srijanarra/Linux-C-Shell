#include "headers.h"
#include "prompt.h"
#include "parse.h"
#include "function.h"

int main()
{
    path = (char *) malloc(1024);
    strcpy(path, "~");
    getcwd(strhome, 100);                       // getting path of home directory
    signal(SIGCHLD, sigchild_handler);          // gets invoked when a child terminates
    head = NULL;                                // head of the linked list used to store bg processes
    while(1) {
        prompt();
        parse();
        for(int i=0; i<narg; i++) {             // parsing each command
            parse_command(tok_commands[i]);
        }
    }
    free(path);
    free(tok_commands);
    free(strcmd);
    free(builtincmds);
    free(str_fgbg);
}