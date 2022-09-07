#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <grp.h>
#include <sys/wait.h>
#include <time.h>

#define DEBUG 0
#define HISTORY "history.txt"
#define MAX_HISTORY 20
