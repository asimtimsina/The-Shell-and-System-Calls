#ifndef FUNC_H
#define FUNC_H





#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <stdbool.h>

size_t size = 512;   //variable to storre size
char Prompt[50] = "prompt";
int no_argument = 0; // store number of arguments

int PATH(char **command); // function declaration to perform the path command
struct sigaction act;  
int def_Execution(char **finalCommand, char **separateComd); // function to execute built in commands.

void totalProcess(char *command); // function that separate different commands and perform the total userinput commands
char **lineParsing(char *command);


///////////
/*
void exits(char *, char **);
bool is_exit(char *);
int semiColon(char *, char **);
void whiteSpace(char *, char **);
void exec_argu(char **);
voide Pipe_args(char** parsed, char** parsedpipe);
*/
/////////


char *myhistory_array[512]; // This stores the list of commands from history
int myhistory_cnt=0; // This stores the number of commands from history 

//Declaring the functions:
int myhistory(); // This prints out the history
int delete_History();// This deletes the history
int change_Directory(char dir[32]); //This executes cd command






#endif