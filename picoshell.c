/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char **cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands with arguments. Each rows 
of cmds are an argv array directly usable for a call to execvp. The first
arguments of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur, the function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain something to help you test your function.

Examples: 
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb/
*/

#include <unistd.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int	i = 0;
	int	fd[2];
	pid_t	pid;
	while (cmds[i])
	{
		if (pipe(fd) < 0)
			exit(1);
		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{}

		i++;
	}
}

int main(void)
{
	char **cmds[4];
	//static char *cmd1[] = {"cat", NULL};
	//static char *cmd2[] = {"cat", NULL};
	//static char *cmd3[] = {"ls", "-l", NULL};
	static char *cmd1[] = {"ls", NULL};
	static char *cmd2[] = {"grep", "l", NULL};
	static char *cmd3[] = {"wc", NULL};
	cmds[0] = cmd1;
	cmds[1] = cmd2;
	cmds[2] = cmd3;
	cmds[3] = NULL;
	int res = picoshell(cmds);
	if (res)
		write(1, "Returned 1 (error)\n", 11);
	else
		write(1, "OK\n", 3);
	return (0);
}
