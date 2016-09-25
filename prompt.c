#include "prompt.h"



void adjust_dir_name(char *abs_dir, char *last_dir)
{
    /*get the last dir name and put it into last_dir*/
    char *p1 = abs_dir;
    char *p2;
    
    while (*p1){
        if (*p1 == '/')
            p2 = p1;
        p1++;
    } // p2 now point to the last '/'
    strncpy(last_dir, p2, sizeof(last_dir));
}


void print_prompt()
{
    char *username = getlogin(); // return a string in static memory, no need for free 
    char hostname[20];
    char last_dir[20];
    gethostname(hostname, sizeof(hostname));
    char *current_dir = get_current_dir_name();
    adjust_dir_name(current_dir, last_dir);
    
    printf("%s@%s: \%s$ ", username, hostname, last_dir);
    free(current_dir);
}

#ifdef DEBUG
int main(void)
{
    print_prompt();
    return 0;
}
#endif