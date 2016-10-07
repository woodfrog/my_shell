#include "prompt.h"


/*get the last dir name and put it into last_dir*/
void adjust_dir_name(char *abs_dir, char *last_dir)
{
    char *p1 = abs_dir;
    char *p2;
    
    while (*p1){
        if (*p1 == '/')
            p2 = p1;
        p1++;
    } // p2 now point to the last '/'
    strncpy(last_dir, p2+1, strlen(p2)); // cannot use sizeof(last_dir) here --> it's the size of ptr

}


void print_prompt()
{
    char *username = getlogin(); // return a string in static memory, no need for free 
    char hostname[20] = "";
    char last_dir[80] = "";
    char cwd[80] = "";
    gethostname(hostname, sizeof(hostname));
    getcwd(cwd, sizeof(cwd));
    adjust_dir_name(cwd, last_dir);
    
    printf("[%s@%s]: %s $ ", username, hostname, last_dir);
}
