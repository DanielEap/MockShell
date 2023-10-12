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
#include "helpers.h"
#include "extern_library.h"

//this function is to help parse the environment variable that is given from getenv("PATH"). It will return the path of the command if it is found in the environment variable
char *env_parse(char *user_command, char *env){
 
    DIR *dir;
    struct dirent *entry;
    char* token_path = strtok(env, ":"); //tokenize the env path
    char* return_path = (char*) malloc(4096); //return path malloc so it can be returned
    int found = 0;
    while(token_path != NULL){
        if ((dir = opendir(token_path)) == NULL){//open the specific dir thats been tokenized
            //perror("opendir() error");
            //continue;
        }
            
        else {
            while ((entry = readdir(dir)) != NULL) { //go through each entry in the open dir
                if(strcmp(entry->d_name, user_command) == 0){ //if matches the user command, then build out the path of the command
                    found = 1;
                   
                    char path[4096];
                    strcpy(path, token_path);
                    strcat(path, "/");
                    strcat(path, user_command);
                    strcpy(return_path, path);
                        //execv(path, arr);
                    
                }
            }
        }
        if(found == 1){
            return return_path;
        }
        //if gotten here, then it is not found in the current dir, try again with the next token
        token_path = strtok(NULL, ":");
    }
    //return null if command does not exist
    return NULL;
}

//called when there is < or >. This function will handle the redirection of the input and output. It execs from this function directly
void redirection(char **command_line, char *path, int background_flag){
    int stdInSave, stdOutSave; //save the std in and out
    char **command = command_line;
    int in = find_special(command, "<"); //check if there is a < or > in the command line
    int out = find_special(command, ">");
    stdInSave = dup(STDIN_FILENO);  //save the std in and out
    stdOutSave = dup(STDOUT_FILENO);
    //if there is a <, then open the file and redirect the input to the file
    if(in != -1){
        int file_in = open(command[in + 1], O_RDONLY, 0777);
        if(file_in == -1){
            perror("open");
            return;
        }
        command[in] = NULL; //set the < in the parsed command line so it can be execed

        
        dup2(file_in, STDIN_FILENO);
        close(file_in);

    }
    //same process for the output
    if(out != -1){
        int file_out = open(command[out + 1], O_WRONLY | O_CREAT, 0644);
        if(file_out == -1){
            perror("open");
            return;
        }
        command[out] = NULL;
        dup2(file_out, STDOUT_FILENO);
        close(file_out);

    }

    pid_t pid;
    //fork and exec
    if((pid = fork()) == -1){
            perror("fork");
        }else if(pid == 0){ //child

            execv(path, command);
        }else{ //parent
            if(background_flag == -1){ //if & is present, then dont wait
                wait2(pid);  
            }else{
                wait(&pid);
                
            }
        }
        //reset the std in and out
        dup2(stdInSave, STDIN_FILENO);
        dup2(stdOutSave, STDOUT_FILENO);
        fflush(stdout);
        fflush(stdin);
        
        close(stdOutSave);
        close(stdInSave);

  
}


//this is called when there is a pipe in the command line. It will handle the piping of the commands. It execs from this function directly
void piping(char **command_line, int background_flag){
   

    //PATH environment variable
    char *path_env = getenv("PATH");
    
    char copy_env[4096]; //creates the copy of the envpath for when it is iterated
    strcpy(copy_env, path_env);
    // number of pipes in the command line
    int pipe_count = 0;
    for (int i = 0; command_line[i] != NULL; i++) {
        if (strcmp(command_line[i], "|") == 0) { //if there is a pipe, increment the pipe count
            pipe_count++;
            command_line[i] = NULL; // if it is a pipe, replace it with null to help with execv
        }
    }

 
    int fd[pipe_count][2];
    //create the pipes
    for (int i = 0; i < pipe_count; i++) {
        pipe(fd[i]);
    }
    //when pipe is called, the fds look like this
    //fd[0][0] = 3;
    //fd[0][1] = 4;

    int args_count = pipe_count + 1;

    //create a pointer to the command line to help with incrementing for exec
    char **p = command_line;
    pid_t pid;
    //this helps for the edge case, i = 0 
    int input_fd = STDIN_FILENO;
    for (int i = 0; i < args_count; i++) {
        int output_fd = STDOUT_FILENO;
        //detects for the edge case at the max args
        if (i < pipe_count) {
            output_fd = fd[i][1]; 
            
        }

        //forking
        if ((pid = fork()) == -1) {
            perror("fork");
        
        } else if (pid == 0) {//child
           

            //if the exec needs the input, then set
            if (input_fd != STDIN_FILENO) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                }
                close(input_fd);
            }

            //same for output
            if (output_fd != STDOUT_FILENO) {
        
                if (dup2(output_fd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                }
                close(output_fd);
            }

            //uses env_parse to find the path of the command as first param and then p which was the pointer to the original command_line that was passed in.
            char *path = env_parse(*p, path_env);
            if(path == NULL){
                perror("path");
            }else{
                execv(path, p);
                perror("execv");
                exit(1);
            }
        } else {// Parent

            // close the pipe that the child isn't using
            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                close(output_fd);
            }

            // increment to get to the next command, so should be after NULL which was set earlier
            while (*p != NULL && strcmp(*p, "|") != 0) {
                p++;
            }
            if (*p == NULL) {
                p++;
            }

            // Set input for next command
            input_fd = fd[i][0];
            //output_fd = fd[i][1];
        }
    }

    // Wait for all child processes to finish
    if(background_flag != -1){
        wait2(pid);
    }else{
        for (int i = 0; i <= pipe_count; i++) {
            //int status;
            if (wait(&pid) == -1) {
                perror("wait");
                exit(1);
            }
        }
    }
        
}
