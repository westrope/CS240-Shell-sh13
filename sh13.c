/*
	sh13.c

	Ian Westrope
	CS 240
	10/2/2013

	HW 2- Make a shell program that uses bash syntax to 
	run command line inputs. The shell parses the input
	and then trys to run the command. Can only run programs
	that can be found in the $PATH

*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void parseLine ();
void sysCall ( char **args);

#define BUFFER 128
#define ARGS 128

int main()
{
	while(1)
	{
		printf( "?:" );							// shell prompt
		parseLine();
	}
}

void sysCall ( char **args )
{
	pid_t childpid;							// child pid number
	int status;

	if( (childpid = fork() ) == -1 ) // fork
	{
		perror("Error in the fork");
		exit(-1);
	}
	else if( childpid == 0 )			// child code
	{
		if( execvp( args[0], args ) < 0 ) // executes command
		{
			perror( "Command failed" );
			exit(-1);
		}
	}
	else if( childpid != wait(&status) ) // parent code 
	{
		perror( "Lost signal with child" );
	}
}

void parseLine ()
{
	char *args[ARGS];							// hold arguments
	char buffer[BUFFER];					// buffer
	int argc = 0;									// argument counter
	int i = 0;										// counter
	char c = getc( stdin );			// get first char from stream

	while( c != EOF )							// loop while not end of line
	{
		while( c != ' ' && c != '\n' ) 
		{
			buffer[i] = c;						// while c isn't a space or newline
			i++;											// get the next char
			c = getc( stdin );
		}

		buffer[i] = '\0';						// c must be a space or newline so append null char
		args[argc] = strdup(buffer); // copy argument in buffer to args
		argc++;											// incriment number of args
		memset( &buffer[0], 0 , sizeof(buffer) ); // reset buffer and counter for next arg
		i = 0;
			
		while( c == ' ' ) c = getc( stdin ); // ignore extra spaces
			
		if( c == '\n' )							// if newline then time to excute
		{	
			args[argc] = NULL;				// set last arg to NULL
			sysCall( args );					// execute command

			int j = 0;								// free memory
			while( j <= (argc-1) )
			{
				free (args[j]);
				args[j] = NULL;
				j++;
			}
		}	

		if( c == '\n' ) return;	// if newline then return from function
	}
}
