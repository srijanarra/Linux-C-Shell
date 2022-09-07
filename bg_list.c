#include "headers.h"
#include "function.h"

// typedef struct bg_node {
//     pid_t pid;
//     char *p_name;
//     struct bg_node *next;
// };

void add_process(char *p_name, pid_t pid) 
{
    struct bg_node *new_node = malloc(sizeof(struct bg_node));
    new_node->p_name = p_name;
    new_node->pid = pid;

    if(head == NULL) {
        head = new_node;
        return;
    }
    struct bg_node *temp = head;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_node;
}

char *search_process(pid_t pid)
{
    if(head == NULL) {
        return NULL;
    }   
    struct bg_node *temp = head;
    while(temp) {
        if(temp->pid == pid) {
            return temp->p_name;
        }
        temp = temp->next;
    }
    return NULL;
}