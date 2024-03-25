#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("No args passed\n");
		return -1;
	} 
	int stat = 0;
	int fds[2];	
	int pid = 0;

	for(int i = 1; i < argc - 1; i++) //arg 0 is the program itself
	{
		pipe(fds); 
		int rc = fork();
		if(rc > 0)
		{
			pid = rc;
			stat = 0;
			waitpid(pid, &stat, 0);
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
		} 
        else if(rc == 0)
		{
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);
			close(fds[0]);

            //load in previous loop if applicable
			execlp(argv[i], argv[i], NULL);

			close(fds[1]);
			close(fds[0]);
			exit(errno);

		} else
		{
			printf("Fork not created\n");
		}
	}
	execlp(argv[argc - 1], argv[argc - 1], NULL);
	exit(errno);
    return 0;
}


