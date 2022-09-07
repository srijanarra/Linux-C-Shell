#include "headers.h"
#include "prompt.h"

void prompt()                                               // prints the prompt everytime it is called
{
    char hostname[100];
    int i = gethostname(hostname, sizeof(hostname));        // to get hostname
    if(i != 0) {
        perror("in hostname");
        exit(1);
    }

    char *username;                                         
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);                      // to get user name
    if (pw)
    {
        username = pw->pw_name;
    }
    if(username == NULL) {
        perror("in getenv");
        exit(1);
    }

    printf("<%s@%s:%s>", username, hostname, path);

}