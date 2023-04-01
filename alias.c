#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


char* get_aliascmnd(char str[256]); //gets user alias command
char *get_aliasname(char str[256]); //name for the command

void input_error();  //void function for error

void exe_command(int index1); //void function for executing command

int delete_a(int index1, int maximum_index); //delete alias
char alias_n[256][15]; //stores alias in array
char alias_cmd[256][15]; //stores alias command in array

int index1 = 0; //for delimiter

char delimit[]="='"; //delimiter

int alias(char Alias_Command[256], int idx)
{
	int a = 0;   //index for arrays to store alias_name and alias_cmd
	char str[256]; //store input
	char* command_pointer; //pointer to store value returned by get_aliascmnd(); 
	char *command_pointer; //pointer to store value returned by get_aliasname()
	char *command[32]; //for strtok() function to store string tokens
	char *delete_pointer; //pointer to store the value of alias to be deleted
	int command_index; //index of alias to be deleted 

	strcpy(str, Alias_Command); //copying to strcpy
	
	int boolean_Value1 = 0, boolean_Value2 = 0, f_Bool = 0; //checking input
	
	//for deleting alias stored
	if(strcmp(str, "alias -c")==0){
		for(int i = 0; i<index1; i++ ){
			strcpy(alias_n[i], "NULL"); 
			strcpy(alias_cmd[i], "NULL"); 
		}
		printf("Stored aliases have been deleted \n");
		index1 = 0;
		return 1;  
	}

	//if alias -r alias_n is entered, this function finds alias_n and deletes ot
	if(strncmp(str, "alias -r ", 9)==0){
		strcpy(delimit, " "); 
		delete_pointer = malloc (sizeof (char) * 15);
		if(delete_pointer == NULL){
			printf("Error. Memory allocation failed. \n"); 
		}
		delete_pointer = get_aliascmnd(str); 
		printf("%s \n", delete_pointer); 
		for(int c=0; c<index1; c++){
			if(strcmp(alias_n[c], delete_pointer )==0){
				f_Bool = 1; 
				command_index = c; 
				index1 = delete_a(command_index, index1); 
				printf("The alias has been deleted.\n");
				delete_pointer = NULL; 
				strcpy(delimit, "='");
				return 1; 
			}
		}  
		if (f_Bool==0){
			printf("The alias you entered does not exist. \n");
		}
	} 

	//if only alias is entered, this function will output every aliases
	if((strlen(str) == 5) && (strcmp(str, "alias") == 0)){
		if (index1 == 0){
			printf("User has not created any aliases. Create atleast one alias before calling $alias command. \n"); 
			return 1; 
		}
		for (int c=0; c<index1; c++){
			printf("%d. alias name: %s    alias command: %s. \n", (c+1), alias_n [c] ,alias_cmd[c]);
			if (c==index1-1){
				return 1; 
			}
		}  
	}

	//if correct input
	if(strncmp(str, "alias ", 6) != 0){
		input_error(); 
		return 1;

	}

	//checking for = in the input
	int indx = strlen(str); 
	for(int i = 0; i<indx; i++){
		if(str[i] =='='){
			boolean_Value1 = 1; 
		}
	}

	int No_Quote = 0; //for quotation marks

	for(int i = 0; i<indx; i++){
		if(str[i] == '\''){
			No_Quote++; 
		}
	}
	if(No_Quote == 2){
		boolean_Value2 = 1; 
	}

	//for invalid input
	if (boolean_Value1 == 0 || boolean_Value2 == 0 ){
		input_error(); 
	}
	//for new input
	command_pointer = malloc (sizeof (char) * 15);
	if (command_pointer == NULL){
		printf("Error!! Memory allocation failed. \n");
	}
	command_pointer = get_aliascmnd(str);
	for(int i =0; i<index1; i++){
		if (strcmp(alias_cmd[i], command_pointer) == 0){
			printf("%s already exists. \n", command_pointer);
			printf("%d. alias name: %s    alias command: %s. \n", (i+1), alias_n [i] ,alias_cmd[i]);
			return 1;
		} 
	}
	strcpy(alias_cmd[index1], command_pointer); 
	
	//storing new 
	command_pointer = malloc (sizeof (char) * 15);
	if (command_pointer == NULL){
		printf("Error. Memory allocation failed. \n");
	}
	command_pointer = get_aliasname(str);
	for(int i =0; i<index1; i++){
		if (strcmp(alias_n[i], command_pointer) == 0){
			printf("%s already exists. \n", command_pointer);
			printf("%d. alias name: %s    alias command: %s. \n", (i+1), alias_n [i] ,alias_cmd[i]);
			return 1; 
		} 
	}
	strcpy(alias_n[index1], command_pointer); 
	
	index1++;  //index ++

	command_pointer = NULL;  //delocating pointer
	command_pointer = NULL;
       	return 1; 	
}
//function for dealing with alias command 
char* get_aliascmnd(char str[256]){
	char *aliascmd; 
	char *command[32]; 
	int i = 0; 
	aliascmd = malloc (sizeof (char) * 15);
	if (aliascmd == NULL){
		printf("Error. Memory allocation failed. \n");
	}
	char* tok = strtok(str, delimit);
	while (tok != NULL) {
		command[i] = tok;
		i++;
		tok = strtok(NULL, delimit);
	}
	aliascmd = command[i-1]; 
	return aliascmd;  
}

//function for dealing with alias name
char *get_aliasname(char str[256]){
	char *alias_n; 
	char *command[32];
	char *cmd2[32];  
	int i = 0, j =0; 
	alias_n = malloc (sizeof (char) * 15);
	if(alias_n == NULL){
		printf("Error. Memory allocation failed. \n");
	}
	char* tok = strtok(str, delimit);
	while (tok != NULL) {
		command[i] = tok;
		i++;
		tok = strtok(NULL, delimit);
	}

	char *tok2 = strtok(command[0], " ");
	while (tok2 != NULL) {
		cmd2[j] = tok2; 
		j++;
		tok2 = strtok(NULL, " ");
	}
	alias_n = cmd2[j-1];
	if(strcmp(alias_n, "alias")==0){
		input_error();  
	}
	return alias_n;  
} 

//printing output
void input_error(){
	printf("Error! Below are accepted form: \n");
	printf("1. Use alias alias_name = 'command' if you want to enter a new alias. \n");
	printf("2. Use alias -r alias_name if you want to delete an alias. \n");
	printf("3. Use alias -c if you want to delete all aliases. \n");
	printf("4. Use alias if you want to list all the aliases.\n");
	printf("Note: Space before or after a command is not accepted. \n");
}
int delete_a(int index1, int maximum_index){
	for(int i = index1; i<=maximum_index-1; i++ ){
		if (i == maximum_index){
			strcpy(alias_n[i], "NULL");  
			strcpy(alias_cmd[i], "NULL");  
		}
		else{
			strcpy(alias_n[i], alias_n[i+1]);  
			strcpy(alias_cmd[i], alias_cmd[i+1]);
		}
	}
	return (maximum_index-1);
}

//executing command based on index
void exe_command(int index1){    //index1
	char *string_3; 
	char *command[15];
	int a = 0; 
	string_3 = malloc(sizeof (char) * 15);
	if (string_3 == NULL){ 
		printf("Memory allocation is failed.\n");
	}
	strcpy(string_3, alias_cmd[index1]);
	char *tok3 = strtok(string_3, " ");
	while (tok3 != NULL) {
		command[a] = tok3;

		a++;
		tok3 = strtok(NULL, " ");
	}
	command[a] = NULL;
	
	string_3 = NULL; //delocating 

	pid_t pid;
	pid=fork();
	
	//for child process
	if (pid == 0){

		execvp(command[0], command);
		printf("%s command in alias '%s' is invalid.\n", command[0], alias_n[index1]);
	}

	//for parent process
	else  
	{
		wait( (int *)0 );
	}
}
