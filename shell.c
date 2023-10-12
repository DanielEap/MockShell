#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "builtins.h"
#include "extern_library.h"
#include "helpers.h"




int main(int argc, char* argv[]){
    char *path_env = getenv("PATH");
    char copy_env[4096]; //creates the copy of the envpath for when it is iterated
    strcpy(copy_env, path_env);
    
    while(1){
        //simulate shell prompt
        printf("DanielSH$ ");


        char line[4096];
        fgets(line ,4096, stdin);//scan in line from user
        strtok(line, "\n"); //trashes the new line character from fgets
        char **command_line;
        command_line = parse(line, " ");
        int i = 0;
        int background_flag = -1; 
        int j = 0;

        //check if & is specifically at the end of line
        while (command_line[j] != NULL) {
            if(strcmp(command_line[j], "&") == 0){
                background_flag = j;
                command_line[j] = NULL;
            }
            j++;
        }
        //logic gate, if first command in the line matches a builtin, then it will execute the builtin
            if(strcmp(command_line[i], "pwd") == 0){
                pwd();
                puts("");
            }
            else if(strcmp(command_line[i], "help") == 0){
                help();  
                puts("");
            }
             
            else if(strcmp(command_line[i], "cd") == 0){
                cd(command_line[i + 1]); // maybe make for when no option because goes to root??
                i++; // to offset the following input that is the dir
                puts("");
            }
            else if(strcmp(command_line[i], "exit") == 0){
                exit(EXIT_SUCCESS);
                
            }else{ //else is for when there is command that is not built in 
                
                char *path;
                path = env_parse(command_line[i], path_env); //get the path of the command

                strcpy(path_env, copy_env); //reset the envpath so a linux command can be execed again
                if(path == NULL){
                    printf("not a valid command"); 
                }else{
                    pid_t pid; 
                    // check for the special characters
                    int special_input_index = find_special(command_line, "<");
                    int special_output_index = find_special(command_line, ">");
                    int special_pipe_index = find_special(command_line, "|");
                    //redirection
                    if(special_output_index != -1 || special_input_index != -1){
                        redirection(command_line, path, background_flag);
                    }else if(special_pipe_index != -1 ) {//piping
                        piping(command_line, background_flag);
                    }else{
                        //if not a special character, then just execute a single command
                        if((pid = fork()) == 0){
                        
                        execv(path, command_line);
                        }else{
                            if(background_flag != -1){
                                wait2(pid);
                            }
                            else{
                                wait(&pid);
                            }
                        }
                    }
                }
                strcpy(path_env, copy_env); //reset the envpath so a linux command can be execed again
                puts("");
            }
            i++;
        //}
        
    }
    return 0;

}

