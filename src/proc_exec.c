#include <unistd.h> // Required for pid_t fork(void); -> from man fork(2); required for exec*() -> man exec(3)
#include <stdio.h> // Required for and standard IO operations (ex. printf(), perror()) 
#include <sys/types.h> // Required for pid_t type -> Advanced Linux Programming (Chapter 3, p. 46)
#include <sys/wait.h> // from man wait(2)

int main() {
   pid_t created_process_pid = fork(); // "When reffering to process IDs [...] always use the pid_t typedef [...]" - Advanced Linux Programming (Chapter 3, p.46) 
    
   if (created_process_pid < 0) {
   	printf("\n(parent process) ERROR! Child process was not created!\n");
	return -1;
   }
   else {
	printf("\nSUCCESS! Child process created!\n");
	if (created_process_pid != 0) {
		printf("This is the parent process.\n");
		printf("(parent process) PID: %d\n", (int) getpid());
		printf("(parent process) Child process ID: %d\n", (int) created_process_pid);	
	}
	if (created_process_pid == 0) {
		printf("Welcome inside the child process!\n");
		printf("(child process) PID: %d\n", (int) getpid());
		printf("(child process) Using exec to change process image. 'ls' binary used: \n");
		execlp("ls", "ls", NULL); // first argument  - file that will be executed
					  // second argument - first argument of the process so argv[0], so it is the name.
					  // execlp is used because 'ls' is a binary that location is stored in the $PATH variable.
		perror("(child process) 'execlp' exited with error:");
		return 1;
	}
	else {
		int status_from_waitpid;
		printf("(parent process) Using 'waitpid' to wait for child process\n");
		waitpid(created_process_pid, &status_from_waitpid, 0); // Suspends execution of the calling thread until child has changed the state
							 // 0 is and 'option' argument meaning no options constants will be used - man wait(2)
		printf("(parent process) Child process exited with %d\n:", WEXITSTATUS(status_from_waitpid)); // WEXITSTATUS is one of the macros used to inspect 'wstatus' - man wait(2)
	}	
   }
}
// Compiled with: gcc -Wall -Wextra -Werror -std=c11
