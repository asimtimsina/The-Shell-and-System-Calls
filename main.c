#include "func.h"
#include "alias.h"

#define MAX_COMM 100

void exits(char *, char **);
bool is_exit(char *);
int semiColon(char *, char **);
void whiteSpace(char *, char **);
void exec_argu(char **);


int main(int argc, char **argv){


  if (argc > 2){
  perror("Error");
  exit(0);
  }
  char *commandLine = malloc(size * sizeof(char)); // cstring array to store the user entered prompt
  if(commandLine = NULL){
  perror("Commandline failed\n");
  exit(1);
  }
  
  
  while(1){
  tcsetpgrp(fileno(stdin), getpgrp());
		act.sa_handler = SIG_IGN;
		assert(sigaction(SIGINT, &act, NULL) == 0);
		assert(sigaction(SIGQUIT, &act, NULL) == 0);
   
  if(argc ==2){
  // batch mode
  }
  else{
  // user interface mode
    printf("%s> ", Prompt); //printing the prompt
    int get = getline(&commandLine, &size, stdin);
    //printf("%s", commandLine);
    commandLine[strcspn(commandLine, "\n")] = 0; // remove \n from commandLine
    
    for(int i = 0; i<256; i++){
				char *temp;
				temp = malloc (sizeof (char) * 15);
				if (temp == NULL){
					printf("Memory allocation failed. \n");}
				strcpy(temp, commandLine);
				if(strcmp(temp, aliasname[i]) == 0){
					executeAliasCmd(i);
					temp = NULL;
				}
			}
    
    // ask user for command
    while (get ==1)
			{
				printf("%s> ",Prompt);
				get=getline(&commandLine, &size, stdin);   
			}

			//required for my history
			if (myhistory_cnt==20){
				myhistory_cnt=0;
			}

			//calling totalProcess and flushing from buffer
			myhistory_array[myhistory_cnt++] = strdup(commandLine);
			fflush(stdout); 
			totalProcess(commandLine);
    }  



}
}















  
/////////////////////////////////////////////////////////////////////////////////
//Asim Timsina
  
  int Path(char **command){ // fucntion to print out the path command 
  	char *delete_line; // variable to store the term to be deleted
	  if(command[1] == NULL){ // just path without + or -
	  	printf("PATH = %s\n", getenv("PATH")); 
	  }

	else if(strcmp(command[1], "+") == 0){ // append to the end of the path
		strcat(getenv("PATH"), ":"); // append colon : to the end of the path
		strcat(getenv("PATH"), command[2]); // append to the new term to end of the path
		setenv("PATH", getenv("PATH"), 1);
	}

	else if(strcmp(command[1], "-") == 0){
		delete_line = strstr(getenv("PATH"), command[2]);// remove user input form path
		delete_line--;
		strncpy(delete_line, "", 1);
		setenv("PATH", getenv("PATH"), 1);
	}
	else{

		printf("PATH = %s\n", getenv("PATH"));//print path
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
// IO redirection function defination


int io_redirection(char **command) 
{
	// var to check the redirection sign
	int less_than =0;
	int	greater_than = 0; 
	
	for(int i=0; i <no_argument; i++)
	{
		if(  strcmp(command[i], "<") == 0)
			less_than++;
	}

	for(int i=0; i <no_argument; i++)
	{
		if(  strcmp(command[i], ">") == 0)
			greater_than++;
	}

	pid_t pid;
	//if condition for IO redirection
	if((less_than > 0 )|| (greater_than > 0) ){
		int fd;
		pid = fork();
		if(pid == 0) {
			if((less_than == 0) && (greater_than > 0) ){ //if > is found
				fd = open(command[2], O_CREAT | O_TRUNC | O_WRONLY, 0600);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			
			
			else if(greater_than == 0 &&less_than > 0){ // if < is found
				fd = open(command[2], O_RDONLY, 0600);
				dup2(fd, STDIN_FILENO);
				close(fd);
			} 
			
			else if(greater_than > 0 && less_than > 0){ //if both > and < is found
				fd = open(command[2], O_RDONLY, 0600);
				dup2(fd, STDIN_FILENO);
				close(fd);
				fd = open(command[2], O_CREAT | O_TRUNC | O_WRONLY, 0600);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			
			
			command[1] = NULL;
			if(execvp(command[0], command) == -1){ //if case for error handling
				perror("child not executing");
			}

		}
		waitpid(pid,NULL,0); 
	} 
	
	
	else{

		pid = fork();
		if (pid > 0) { // parent case
		setpgid(pid, pid);
			signal(SIGTTOU, SIG_IGN);
			tcsetpgrp(fileno(stdin), getpgid(pid));
			wait(NULL);

			int random;
			waitpid(pid, &random, 0); 
			while (!WIFEXITED(random) && !WIFSIGNALED(random))
			{
				waitpid(pid, &random, 0);
			}
 			                        
		}
		
		
		else if(pid == 0){ //else if case for child
			setpgrp();
			tcsetpgrp(fileno(stdin), getpgid(pid));
			act.sa_handler = SIG_DFL;
			assert(sigaction(SIGINT, &act, NULL) == 0);
			assert(sigaction(SIGQUIT, &act, NULL) == 0);
			execvp(command[0], command);
			printf("execlp failed\n");
			exit(EXIT_FAILURE);

			exit(1);
		}

		else{ //error case
			perror("fork");
			exit(EXIT_FAILURE); 
		}
		
	}
	
	return 1;

}

/////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------------------------
//exit
//returns number of semicolon(s)

void exec_argu(char **c)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "Unable to create child for execution\n");
		return;
	}

	if (pid == 0) //child
	{
		for (int i = 0; i < sizeof(c); i++)
			if (execvp(c[i], c) < 0)
				fprintf(stderr, "\nCould not execute command...\n");

		exit(0); //end child
	}
	else			//parent
		wait(NULL); //wait for child to finish
	printf("\n");
	return;
}



int semiColon(char *str, char **temp)
{
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		temp[i] = strsep(&str, ";");
		if (temp[i] == NULL)
			break;

		if (strlen(temp[i]) == 0)
			i--;
		else
			count++;
	}

	if (temp[1] == NULL)
		return 0; //if no semicolon was found
	else
		return count; //semicolon found and the amount
}
//seperates by whiteSpace
void whiteSpace(char *str, char **comm)
{
	int n = 0;
	//find the first instance of null in comm array
	while (n < MAX_COMM)
	{
		if (comm[n] == NULL)
			break;
		n++;
	}

	for (int i = n; i < MAX_COMM; i++)
	{
		comm[i] = strsep(&str, " ");

		if (comm[i] == NULL)
			break;
		if (strlen(comm[i]) == 0)
			i--;
	}
}







//////////

void exits(char *in, char **comm)
{
	char *temp[5];

	//count how many commands need to be executed
	int count = semiColon(in, temp);

	if (count == 0) //if only one command
	{
		printf("Exiting...\n"); //exiting
		exit(0);
	}

	for (int i = 0; i < count; i++)
	{
		
		if (strcmp(temp[i], "exit") != 0 && strcmp(temp[i], " exit") != 0 && strcmp(temp[i], "exit ") != 0)
		{
			comm[0] = '\0';
			//seperate by white whiteSpace
			whiteSpace(temp[i], comm);
			exec_argu(comm);
		}
	}

	printf("Exiting...\n");
	exit(0);
}




bool is_exit(char *input)
{
	bool exit = false;

	for (int i = 0; i < 9999; i++)
	{
		if (input[i] == 'e' && input[i + 1] == 'x' && input[i + 2] == 'i' && input[i + 3] == 't')
			exit = true;

		if (input[i] == '\0')
			break;
	}

	return exit;
}








/////////////////////////////////////////////////////////////////////////////
void totalProcess(char *command) 
{
	char **mul_commands;
	int no_commands = 0;
	char *temp;
	char **sep_commands;
	sep_commands = malloc(size * sizeof(char*));
	if (sep_commands==NULL){
		exit(1); // error handling 
	}

	temp = strdup(command);

	char *var;
	var = strtok(temp, ";");
	while (var != NULL){
		sep_commands[no_commands] = var;
		if(strncmp(sep_commands[no_commands], "alias", 5)==0){
			char *p;
			p = malloc(sizeof(char) * 256);
			strcpy(p, sep_commands[no_commands]);
			alias(p, index1);
			p = NULL;
		}
		sep_commands = realloc(sep_commands, size * sizeof(char*));
		var = strtok(NULL, ";");
		no_commands++;
	}

	for(int i=0; i<no_commands; i++){
		if(( strcmp(sep_commands[i], " exit ") == 0)){	
			i++;						 	
			mul_commands = lineParsing(sep_commands[i]);
			def_Execution(mul_commands, sep_commands);
			free(command);
			free(mul_commands);
			exit(0);
		}
		else
		{
			mul_commands = lineParsing(sep_commands[i]);
			def_Execution(mul_commands, sep_commands);
			free(command);
			free(mul_commands);
		}
	}
	return;     
}	


////////////////////////////////////////////////////////////////////
char **lineParsing(char * Block)
{
	char white_space[] = {" \t\r\n\a"};     
	char **finalCommand = malloc(size * sizeof(char*));
	if (finalCommand==NULL)
	{
		printf("Memory Allocation failed.\n"); // error handling 
		exit(1);
	}
	char *temp;
	int count = 0;

	temp = strtok(Block, white_space);
	while(temp != NULL)
	{
		finalCommand[count] = temp;
		count++;
		finalCommand = realloc(finalCommand, size * sizeof(char*));
		temp = strtok(NULL, white_space);
	}
	finalCommand[count] = NULL;
	no_argument = count;

	return finalCommand;
}

/////////////////////////////////////////////
int def_Execution(char **finalCommand, char **separateComd){


	if( strcmp(finalCommand[0],"exit") == 0){ 	// if exit
		exit(-1);
	}
	//check for cd, exit, path, myhistory  
	if (strcmp(finalCommand[0],"path") == 0) {	// check for cd, path, myhistory , exit 
		return Path(finalCommand);
	} 
	
	else if(strcmp(finalCommand[0],"customize") == 0){
		printf("Please enter the new prompt that you want to customize : ");
		char temp[50], x;
		scanf("%s", temp); 
		scanf("%c", &x);
		strcpy(Prompt,temp);

		return 1;
	}


	else if(strcmp(finalCommand[0],"myhistory")==0){ // for myhistory
		if(finalCommand[1] == NULL){
			myhistory();
		}
		else if (strcmp(finalCommand[1],"-c")==0){
			delete_History();
		}

		else if( strcmp(finalCommand[1],"-r")==0){
			char n;
			strcpy(&n, finalCommand[2]);
			int t = n-'0';


			printf("[%d] %s\n",t, myhistory_array[t-1]);

		}
	}
	
	else if (strcmp(finalCommand[0],"cd")==0) // for cd
	{

		if(finalCommand[1] == NULL){
			change_Directory("/home");
		}
		else {

			change_Directory(finalCommand[1]);
		}
	}

	//redirectiom
	io_redirection(finalCommand);

	return 1;


}
