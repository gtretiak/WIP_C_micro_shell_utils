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
close all the open fds before). Otherwise the function must wait all child
processes and return 0. 

Examples: 
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb/
*/

#include <unistd.h>
#include <sys/wait.h>
//#include <sys/types.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int	i = 0;
	int	fd[2];
	int	fd_to_use = 0;
	int	last_ret;
	int	total_ret = 0;
	pid_t	pid;
	while (cmds[i]) // while cmds
	{
		if (cmds[i + 1]) // if it isn't the last
		{
			if (pipe(fd) < 0)
			{
				if (fd_to_use != 0)
					close(fd_to_use);
				return (1);
			}
		}
		else // otherwise we don't need a pipe
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		pid = fork();
		if (pid < 0)
		{
			if (fd[0] != -1 || fd[1] != -1)
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (fd_to_use != 0)
				close(fd_to_use);
			return (1);
		}
		if (pid == 0)
		{
			if (fd_to_use != 0) // not the first cmd, so we need previous output
			{
				if (dup2(fd_to_use, 0) < 0)
					exit(1);
				close(fd_to_use);
			}
			if (fd[1] != -1) // not the last cmd, so we need to redirect
			{
				if (dup2(fd[1], 1) < 0)
					exit(1);
				close(fd[1]);
				close(fd[0]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (fd[1] != -1)
			close(fd[1]);
		if (fd_to_use != 0)
			close(fd_to_use);
		fd_to_use = fd[0];
		i++;
	}
	while (wait(&last_ret) > 0)
	{
		if (WIFEXITED(last_ret) && WEXITSTATUS(last_ret) != 0)
			total_ret = 1;
		else if (!WIFEXITED(last_ret))
			total_ret = 1;
	}
	return (total_ret);
}

int main(void)
{
	char **cmds[4];
	static char *cmd1[] = {"ls", NULL};
	static char *cmd2[] = {"grep", "l", NULL};
	static char *cmd3[] = {"wc", NULL};
	/* Another test.
	 * This time ls doesn't require input
	 * This means that if cat cmds aren't ended with Ctrl+D (EoF), they send text to ls
	 * which leads to error.
	static char *cmd1[] = {"cat", NULL};
	static char *cmd2[] = {"cat", NULL};
	static char *cmd3[] = {"ls", "-l", NULL};
	*/
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
