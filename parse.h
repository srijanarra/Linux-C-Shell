#ifndef PARSE_H
#define PARSE_H

char *tok_commands[1024];     // to store tokenized commands
int narg;               // to store number of arguments
int nstr;               // to store number of strings in a command
char *strcmd[1024];      // to store each string in an already tokenized string (command)

void parse();

#endif