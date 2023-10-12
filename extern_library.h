#ifndef EXTERN_LIBRARY_H
#define EXTERN_LIBRARY_H

char *env_parse(char *user_command, char *env);
void redirection(char **command_line, char *path, int background_flag);
void piping(char **command_line, int background_flag);

#endif