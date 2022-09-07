#include "headers.h"
#include "function.h"

void discover(char *disc_args[], int n)
{
    char *target_dir = (char *) malloc(1024);
    if((disc_args[1][0] == '-') || (disc_args[1][0] == '\"')) {
        strcpy(target_dir, ".");
    }
    else {
        target_dir = disc_args[1];
    }

    int d = 0, f = 0;
    char *filename = (char *) malloc(1024);
    filename = NULL;

    for(int i=0; i<n; i++) {
        if(strcmp(disc_args[i], "-d") == 0) {
            d = 1;
        } 
        if(strcmp(disc_args[i], "-f") == 0) {
            f = 1;
        } 
        if(disc_args[i][0] == '\"') {
            filename = disc_args[i];
        }
    }
    if(d == 0 && f == 0) {
        d = 1; f = 1;
    }
    disc_recursive(target_dir, filename, d, f);

    free(filename);
    // free(target_dir);
}


void disc_recursive(char *current_dir, char *filename, int d, int f)
{
    char *path = (char *) malloc(1024);
    struct dirent *d_curr;
    DIR *p_curr = opendir(current_dir);
    if(p_curr == NULL) {
        perror("Error in opendir: ");
        return;
    }
    while((d_curr = readdir(p_curr)) != NULL) {

        int isdir = 0;
        sprintf(path, "%s/%s", current_dir, d_curr->d_name);
        struct stat sb;
        int k = stat(path, &sb);
        if(k < 0) {
            perror("Error in stat in disc_recursive: ");
            return;
        }
        if((sb.st_mode & S_IFMT) == S_IFDIR) {
            isdir = 1;
        }
        if(d == 1 && isdir == 1) {
            int ret_value = match(d_curr->d_name, filename);
            if(ret_value == 0) {
                printf("%s\n", path);
            }
        }
        if(f == 1 && isdir == 0) {
            int ret_value = match(d_curr->d_name, filename);
            printf("path: %s\n", path);
            printf("ret_value: %d\n", ret_value);
            if(ret_value == 0) {
                printf("%s\n", path);
            }
        }

        if(isdir == 1 && (strcmp(d_curr->d_name, ".") != 0) && (strcmp(d_curr->d_name, "..") != 0)) {
            disc_recursive(path, filename, d, f);
        }
    }
        

    free(path);
}

int match(char *dir_or_file, char *filename) 
{
    if(filename == NULL) {
        return 0;
    }
    // printf("")
    
    return strcmp(dir_or_file, filename);
    
}