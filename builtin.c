#include "headers.h"
#include "function.h"
#include "parse.h"
#include "prompt.h"

void parse_command(char *strarg)                                                        // parsing each command
{
    int len = strlen(strarg);
    int end_fg = 1;
    int fg = 0, bg = 1;
    for(int i = len-1; i>=0; i--) {                                                     // checking if the last character is '&'
        if((strarg[i] == ' ') || (strarg[i] == '\t') || (strarg[i] == '\n')) {        
            continue;
        }
        else if(strarg[i] == '&') {                                                     
            end_fg = 0;
            break;
        }
        else break;
    }

    for(int i=0; i<200; i++) {
        str_fgbg[i] = (char *) malloc(1024);
    }

    char *amp_parse = strtok(strarg, "&");                                              // tokenizing with '&'
    int n_fgbg = 0;
    while(amp_parse) {
        str_fgbg[n_fgbg++] = amp_parse;
        amp_parse = strtok(NULL, "&");
    }
    
    for(int i=0; i<1024; i++) {
        strcmd[i] = (char *) malloc(1024);
    }

    for(int i=0; i<1024; i++) {
        strcpy(strcmd[i],"\0");
    }

    char *p_bg;                                 
    int nstr_bg;
    for(int i=0; i<n_fgbg-1; i++) {                                                     // parsing first n-1 commands 
        p_bg = strtok(str_fgbg[i], " \t\n");
        nstr_bg = 0;
        while(p_bg) {
            strcmd[nstr_bg++] = p_bg;
            p_bg = strtok(NULL, " \t\n");
        }
        
        check(strcmd, nstr_bg, bg);
    }

    char *p_fg;
    int nstr_fg;

    if(end_fg == 1) {                                                                   // if nth command is a foreground process, 
        p_fg = strtok(str_fgbg[n_fgbg-1], " \t\n");
        nstr_fg = 0;
        while(p_fg) {
            strcmd[nstr_fg++] = p_fg;
            p_fg = strtok(NULL, " \t\n");
        }

        check(strcmd, nstr_fg, fg);

    }
    else {
        p_bg = strtok(str_fgbg[n_fgbg-1], " \t\n");
        nstr_bg = 0;
        while(p_bg) {
            strcmd[nstr_bg++] = p_bg;
            p_bg = strtok(NULL, " \t\n");
        }
        
        check(strcmd, nstr_bg, bg);
    }
    
}


void check(char *strcmds[], int len, int flag)        // function to check if the given command is builtin
{
    if(isbuiltin(strcmds[0]) == 1) {
        builtin(strcmds, len);
    }
    else {
        sys_commands(strcmds, len, flag);
    }

}


void builtin(char *arrcmd[], int n)         
{
    if(strcmp(arrcmd[0], "cd") == 0) cd(arrcmd, n);
    else if(strcmp(arrcmd[0], "pwd") == 0) pwd(arrcmd);
    else if(strcmp(arrcmd[0], "echo") == 0) echo(arrcmd, n);
    else if(strcmp(arrcmd[0], "ls") == 0) ls(arrcmd, n);
    else if(strcmp(arrcmd[0], "pinfo") == 0) pinfo(arrcmd, n);
    else if(strcmp(arrcmd[0], "history") == 0) print_history(arrcmd, n);
    else if(strcmp(arrcmd[0], "discover") == 0) discover(arrcmd, n);
}

int compare(const void *a, const void *b) 
{
    return strcmp(*(char **)a, *(char **)b);
}

void ls(char *lsarg[], int n)
{
    bool l = 0, a = 0;

    for(int i=1; i<n; i++) {
        if(strcmp(lsarg[i], "-l") == 0) l = 1;
        else if(strcmp(lsarg[i], "-a") == 0) a = 1;
        else if((strcmp(lsarg[i], "-la") == 0) || (strcmp(lsarg[i], "-al") == 0)) {
            l = 1; a = 1;
        }
    }

    getcwd(currdir, 1000);
    char *filearr[20], *dirarr[20];
    for(int i=0; i<20; i++) {
        filearr[i] = (char*) malloc(200 * sizeof(char));
        dirarr[i] = (char*) malloc(200 * sizeof(char));
    }

    int i_file = 0, i_dir = 0;

    for(int i=1; i<n; i++) {
        if(lsarg[i][0] == '-') continue;
        if(strcmp(lsarg[i],"~") == 0) {
            strcpy(lsarg[i], strhome);
        }
        struct stat sb;
        int j = stat(lsarg[i], &sb);
        if(j < 0) {
            perror("stat returns error in ls 1: ");
            exit(1);
        }

        if((sb.st_mode & S_IFMT) == S_IFDIR) {
            strcpy(dirarr[i_dir++], lsarg[i]);
            if(DEBUG) {
                printf("adding directory %s\n", dirarr[i_dir-1]);
            }
            
        }
        else {
            strcpy(filearr[i_file++], lsarg[i]);
            if(DEBUG) {
                printf("adding file %s\n", filearr[i_file-1]);
            }
        }
    }

    // TODO: sorting
    if((i_file == 0) && (i_dir == 0)) {
        strcpy(dirarr[i_dir++], ".");
    }

    qsort(dirarr, i_dir, sizeof(char *), compare);
    qsort(filearr, i_file, sizeof(char *), compare);

    for(int i=0; i<i_file; i++) {
        print_details(filearr[i], currdir, l, a);
    }


    if(i_file && i_dir) printf("\n");

    for(int i=0; i<i_dir; i++) {
        struct dirent *d_curr;
        DIR *p_curr = opendir(dirarr[i]);
        if(p_curr== NULL) {
            printf("%s\n", dirarr[i]);
            perror("opendir returns error in ls: ");
            exit(1);
        }
        if(i != 0) printf("\n");
        printf("%s:\n", dirarr[i]); // print directory name
        while((d_curr = readdir(p_curr)) != NULL) {
            char *dir_path;
            print_details(d_curr->d_name, dirarr[i], l, a);
        }
    }

    for(int i=0; i<20; i++) {
        free(dirarr[i]);
        free(filearr[i]);
    }   
}




void print_details(char *filename, char *dir, bool l, bool a)
{
    char filepath[1024];   
    strcpy(filepath, dir);
    strcat(filepath, "/");
    strcat(filepath, filename);

    if((a == 0) && (filename[0] == '.')) return;
    
    struct stat sb;
    int j = stat(filepath, &sb);
    if(j < 0) {
        printf("%s\n", filepath);
        perror("stat returns error in print_details: ");
        exit(1);
    }

    if(l == 1) {
        switch (sb.st_mode & S_IFMT)
        {
            case S_IFREG: printf("-"); break;
            case S_IFDIR: printf("d"); break;
        }
        printf((sb.st_mode & S_IRUSR) ? "r" : "-");
        printf((sb.st_mode & S_IWUSR) ? "w" : "-");
        printf((sb.st_mode & S_IXUSR) ? "x" : "-");
        printf((sb.st_mode & S_IRGRP) ? "r" : "-");
        printf((sb.st_mode & S_IWGRP) ? "w" : "-");
        printf((sb.st_mode & S_IXGRP) ? "x" : "-");
        printf((sb.st_mode & S_IROTH) ? "r" : "-");
        printf((sb.st_mode & S_IWOTH) ? "w" : "-");
        printf((sb.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %ld", sb.st_nlink);
        printf(" %s", getpwuid(sb.st_uid)->pw_name);
        printf(" %s", getgrgid(sb.st_gid)->gr_name);
        printf(" %10ld", (long)sb.st_size);

        // printf(" %ld ", sb.st_mtime);       // TODO: format time
        time_t curr_time = time(NULL);
        struct tm *t = localtime(&curr_time);
        int present_month = t->tm_mon;
        int present_year = 1970 + t->tm_year;

        t = localtime(&sb.st_mtime);
        int modif_month = t->tm_mon;
        int modif_year = 1970 + t->tm_year;

        char *format = ((modif_year == present_year) && (modif_month >= (present_month-6))) ? " %b %e\t%H:%M " : " %b %e\t%Y ";
        char t_buf[200];
        strftime(t_buf, sizeof(t_buf), format, t);
        printf("%s", t_buf);
    }

    if((sb.st_mode & S_IFMT) == S_IFDIR) {
        printf(BLU "%s\n" RESET, filename);
        return;
    }
    if((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH)) {
        printf(GRN "%s\n" RESET, filename);
        return;
    }
    if((sb.st_mode & S_IFMT) == S_IFREG) {
        printf(YEL "%s\n" RESET, filename);
    }

}

// char* formatted_time(time_t timestamp)
// {

// }

void cd(char *cdarg[], int len)         // executing cd command
{
    char *t = (char *) malloc(512);
    if(len > 2) {
        errno = 5;
        perror("in cd");
    }
    else if(len == 1) {
        if(chdir(strhome) != 0) perror("in cd, chdir");
    }
    else {
        if(strcmp(cdarg[1], ".") == 0) {
            getcwd(prevdir, 1000);
            if(chdir(".") != 0) {
                perror("in cd .");
                return;
            }
        }
        else if(strcmp(cdarg[1], "..") == 0) {
            getcwd(prevdir, 1000);
            if(chdir("..") != 0) {
                perror("in cd ..");
                return;
            }
        }
        else if(strcmp(cdarg[1], "~") == 0) {
            getcwd(prevdir, 1000);
            if(chdir(strhome) != 0) {
                perror("in cd ~");
                return;
            }
        }
        else if(strcmp(cdarg[1], "-") == 0) {
            strcpy(t, prevdir);
            getcwd(prevdir, 1000);
            if(chdir(t) != 0) {
                perror("in cd -");
                return;
            }
            printf("%s\n", getcwd(buf, 1000));
        }
        else {
            getcwd(prevdir, 1000);
            if(chdir(cdarg[1]) != 0) {
                perror("in cd");
                return;
            }
        }
    }
    

    char *s1 = (char *) malloc(1024 * sizeof(char));
    getcwd(s1, 1000);
    char *ptr;
    ptr = strstr(strhome, s1);
    if(ptr == NULL) {
        char temp[1000];
        getcwd(temp, 1000);
        char *temp2;
        temp2 = strrchr(temp, '/');
        char *temp3 = (char *) malloc(512);
        strcpy(temp3, path);
        strcat(temp3, temp2);
        strcpy(path, temp3);
        // free(temp2);
        free(temp3);
    }
    else if(strcmp(strhome, s1) == 0) {
        strcpy(path, "~");
    }
    else {
        getcwd(path, 1000);
        strcpy(path, s1);
    }

    free(t);
    free(s1);
    
}

void pwd(char *pwdarg[])                    // executing pwd command
{
    printf("%s\n", getcwd(buf, 1000));
}

void echo(char *echoarg[], int len)         // executing echo command
{
    for(int i=1; i<len; i++) {
        printf("%s ", echoarg[i]);
    }
    printf("\n");
}

int isbuiltin(char *str)                    // checking what type of builtin command the command is
{
    for(int i=0; i<100; i++) {
        builtincmds[i] = (char *) malloc(1024);
    }
    int num_builtin = 7;
    builtincmds[0] = "cd";
    builtincmds[1] = "pwd";
    builtincmds[2] = "echo";
    builtincmds[3] = "ls";
    builtincmds[4] = "pinfo";
    builtincmds[5] = "history";
    builtincmds[6] = "discover";

    for(int i=0; i<num_builtin; i++) {
        if(strcmp(str,builtincmds[i]) == 0) {
            return 1;
        }
    }
    return 0;
}