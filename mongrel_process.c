#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define PID_FILE_BASE "jmongrel"
#define DEBUG 1

/**
 * return a char pointer to the string base 10 representation of the int passed in
 * param a int to convert
 * param the number of characters in the int passed in, will truncate if the correct size is not given
 **/
char * i_to_str(const int, const int);
char * i_to_str(const int i, const int size){
	char *strp = malloc(size);
	(void)snprintf(strp, size, "%d", i);
	#ifdef DEBUG
		puts("i_to_str=");
		puts(strp);
	#endif
	return strp;
}

void print_args(char *const [], int);
void print_args(char *const tmp[], int len){
	int i = 0;
	(void)printf("[");
	for(; i < len; i++){
		int length =  strlen(tmp[i]);
		(void)printf(" %s(%d)", tmp[i], length);
	}
	(void)printf("]\n");
}

/**
 * parameters
 * 	a char string of the port
 *  a char string of the pid
 * return 0 on success, otherwise > 0
 **/
int write_pid_file(const char *const, const char *const);
int write_pid_file(const char *const port, const char *const pid){
	FILE *fp = NULL;
	int size = strlen(PID_FILE_BASE) * 2;
	char file_name[size];
	strncat(file_name, PID_FILE_BASE, size);
	strncat(file_name, ".", 1);
	strncat(file_name, port, strlen(port));
	strncat(file_name, ".pid", 4);
	puts("file_name=");
	puts(file_name);
	fp = fopen(file_name, "w");
	if(fp == NULL){
		perror("failed to open file");
		return 1;
	}
	//write_val = fputs(pid, fp);
	fputs(pid, fp);
	fputs("\n", fp);
	//int write_val = -1;
	//write_val = fputs(pid, fp);
	//write_val = fputs("\n", fp);
/*
	(void)printf("write_val=%d", write_val);
	if(fputs(pid, fp) != 'EOF'){
		perror("fputs");
		return 1;
	}
*/
 	return fclose(fp);
}


int main(int argc, char **argv){
	const char *const command = "/Users/dmknopp/jruby/bin/jruby";
	int i, port;
	printf("command = %s\n", command);
	const int max_procs = 1;
	i = -1;
	port = 3000;
	const pid_t pid = getpid();
	(void)printf("parent pid=%d\n", pid);
	for(i = 0; i < max_procs; i++){
		port += 1;
		char *const myport = i_to_str(port, 12);
		char *const args[] = { "jruby", "-S", "mongrel_rails", "start", "-p", myport, NULL };
		int len = sizeof(args)/sizeof(args[0]);
		(void)printf("len=%d\n", len);
		int pid = -1;
		int status = -1;
		pid = fork();
		(void)printf("pid after fork=%d\n", pid); 
		if(pid >= 0){
			if(pid == 0){
				const pid_t child_pid = getpid();
				#ifdef DEBUG
					(void)printf("child proc executing program w/ pid=%d\n", child_pid);
					(void)printf("child parent proc executing program w/ pid=%d\n", getppid());
					(void)puts("writing pid file...");
				#endif
				int success = write_pid_file(myport, i_to_str(child_pid, 12));
				execv(command, args);
				(void)puts("child proc done executing program\n");
				perror(*args);
				exit(EXIT_SUCCESS);
			}else{
				(void)free(myport);
				#ifdef DEBUG
					(void)printf("waiting for child process of pid=%d to return...\n", pid);
				#endif
				while(wait(&status) != pid);
				#ifdef DEBUG
					(void)puts("child process returned\n");
				#endif
			}
		}
		else{
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
