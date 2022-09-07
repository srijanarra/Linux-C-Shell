#ifndef FUN_H
#define FUN_H

#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

struct bg_node;

typedef struct bg_node {
    pid_t pid;
    char *p_name;
    struct bg_node *next;
} bg_node;

struct bg_node *head;

char buf[1024];
char *builtincmds[100];
char strhome[1024];
char prevdir[1024];
char currdir[1024];
char *str_fgbg[200];

// char *slash = "/";

int isbuiltin(char *str);
void parse_command(char *argstr);
void builtin(char *cmdarr[], int n);
void pwd(char *pwdarg[]);
void check(char *strcmds[], int len, int flag);
void echo(char *echoarg[], int len);
void cd(char* cdarg[], int l);
void ls(char *lsarg[], int n);
void print_details(char *filename, char *dir, bool l, bool a);
void pinfo(char *pinfoarg[], int n);
void sys_commands(char *strcmds[], int len, int flag);
void pinfo(char *pinfo_cmds[], int n);
void sigchild_handler(int n);
void add_process(char *p_name, pid_t pid);
char* search_process(pid_t pid);
void store_history(char *command);
void print_history(char *his_args[], int n);
void discover(char *disc_args[], int n);
void disc_recursive(char *current_dir, char *filename, int d, int f);
int match(char *dir_or_file, char *filename);

#endif