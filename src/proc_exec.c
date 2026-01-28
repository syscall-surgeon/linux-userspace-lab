#define _POSIX_C_SOURCE 200809L
#include <unistd.h> // Required for pid_t fork(void); -> from man fork(2); required for exec*() -> man exec(3)
#include <stdio.h> // Required for and standard IO operations (ex. printf(), perror()) 
#include <sys/types.h> // Required for pid_t type -> Advanced Linux Programming (Chapter 3, p. 46)
#include <sys/wait.h> // from man wait(2)

int main() {
   pid_t created_process_pid = fork(); // "When reffering to process IDs [...] always use the pid_t typedef [...]" - Advanced Linux Programming (Chapter 3, p.46) 
    
   if (created_process_pid < 0) {
   	perror("\n(parent process) ERROR! Child process was not created!\n");
	return 1; // exit code is unsigned 8-bit int - (-1) = 255
   }
   else {
	printf("\nSUCCESS! Child process created!\n");
	if (created_process_pid != 0) {
		fprintf(stderr,"This is the parent process.\n");
		fprintf(stderr,"(parent process) PID: %d\n", (int) getpid());
		fprintf(stderr,"(parent process) Child process ID: %d\n", (int) created_process_pid);	
	}
	else if (created_process_pid == 0) {
		fprintf(stderr,"Welcome inside the child process!\n");
		fprintf(stderr,"(child process) PID: %d\n", (int) getpid());
		fprintf(stderr,"(child process) Using exec to change process image. 'ls' binary used: \n");
		execlp("ls", "ls", NULL); // first argument  - file that will be executed
					  // second argument - first argument of the process so argv[0], so it is the name.
					  // execlp is used because 'ls' is a binary that location is stored in the $PATH variable.
		perror("(child process) 'execlp' exited with error:");
		return 1;
	}
	else {
		int status_from_waitpid;
		fprintf(stderr,"(parent process) Using 'waitpid' to wait for child process\n");
		waitpid(created_process_pid, &status_from_waitpid, 0); // Suspends execution of the calling thread until child has changed the state
							 // 0 is and 'option' argument meaning no options constants will be used - man wait(2)
		if (WIFEXITED(status_from_waitpid)) {  // WIFEXITED Returns 'true' if child terminate normally. From man wait(2) and Linux Programming Interface template
			printf("(parent process) Child process exited normally with %d\n:", WEXITSTATUS(status_from_waitpid)); // WEXITSTATUS is one of the macros used to inspect 'wstatus' - man wait(2)
		} else if (WIFSIGNALED(status_from_waitpid)) {
			fprintf(stderr,"(parent process) Child killed by SIG %d\n", WTERMSIG(status_from_waitpid));
		} else if (WIFSTOPPED(status_from_waitpid)) {
			fprintf(stderr,"(parent process) Child stopped by SIG %d\n", WTERMSIG(status_from_waitpid));
		} else {
			fprintf(stderr,"(parent process) Child process did not exit normally\n");				
		}	

		
	}	
   }
}
// Compiled with: gcc -Wall -Wextra -Werror -std=c11
