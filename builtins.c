#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "builtins.h"

void cd(char *dir){
    if(chdir(dir) != 0){//cd is executed on this line
        perror("cd failed");
    }else{
        char s[4096];
        printf("%s\n", getcwd(s,4096));
    }
}

void *pwd(){
    char s[4096];
    printf("%s", getcwd(s,4096));
}
//execs on the help.txt that is in the same directory as the shell
void help(){
    __pid_t pid;
    pid = fork();
    if(pid == 0){
        char *args[] = {"more", "help.txt", NULL};
        execv("/bin/more", args);
    }else{
        //wait(NULL);
    }
   
//just simply exit from shell program, not terminal
}
void exit2(){
    exit(EXIT_SUCCESS);
}
//this is called when there is the & at the end of the line
void wait2(pid_t pid){
    int status;
    
    waitpid(pid, NULL, WNOHANG);
    


     
}