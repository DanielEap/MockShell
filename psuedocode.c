/*

#includes libraries and header files for the built in functions

main{

    print stataement to look like currently in shell

    fgets to scan in the line for all the potential args
    string = fgets from stdin

    use the parse function provided to break into separate strings
    char **wordlist = parse(string, " ")

    while(wordlist[index] != NULL){
        switch case(the current string being processed equals a keyword command or 
            functionality, using strcmp, do for all commands){
            
            command case :
                check for the other functionalities
                //if the next string following the command is equal to one of
                //the redirections, then process it
                if(wordlist[index + 1] == |, >, < , &, && index+1 exists)
                    implement this functionality with the provided find_special() function {
                    this is where the forking and execing would happen

                    if pipe{ // |
                        int array = n-1 pipes for n amount of processes
                        left side of pipe is the input and right is the output
                        int filedescriptors[];
                        call the pipe(filedescriptors) function
                        for(i until n processes){
                            pid = fork)()
                            if child(){
                                dup2(change the stdout to the write end of the pipe)
                                close(read end)
                                for this certain command, send its output into the pipe
                                use write()
                                write(fd[1], the thing to output (realistically should always
                                    be some sort of string), sizeof());
                            }
                        }
                    if redirection < > {
                        example for < specifically
                        use fgets(the thing on the right of the symbol to which were outputing , 
                            sizeinput, stdin which will be the input)


                        or another option maybe using dup2
                        file = open(filename)
                        dup2(file, stdout)
                        call the function and should redirect to the file
                        revert back to original descriptors fflush()
                    }


                    }
                }
        }
    }



}




/////BUILT IN FUNCTIONS
char *pwd(){
    path = getcwd();
    
    create new char pointer and malloc it to the size of the path;
    strcpy(path_to_return, path);
    return path_to_return
}

void help(){
    pid = fork()
    if child{
        list[] = {more, text file, null}
        exec(path for the more function, list);
    }
    // the child will take in the text file and place it into more 
    sending it to STDOUT
    else the parent {
        wait to reap 
    }
}

void cd(char *directory to change in to){
    use chdir()

    if(chdir(dir) does not exist, != 0 ){
        send an error 
    }else{
        chdir(dir);
        maybe print out that new directory path using getcwd
    }
    //in reality, this should keep the user in the new directory in main()
    until main is exited
}

void exit(){
    should just be a simple call using built in exit() syscall 
}

//this function should be called by the parent and realistically called every time fork is called
void wait2(){
    
    call waitpid(use the specific child's pid for the first param)
}









*/