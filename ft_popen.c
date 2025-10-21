/* Allowed functions: pipe, fork, dup2, execvp, close, exit

write the following function:
	
	int	ft_popen(const char *file, char *const argv[], char type)

The function must launch the executable file with the arguments argv (using execvp).
If the type is 'r' the function must return a file descriptor connected to the output of the command.
If the type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

example:

int main() {
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

	char *line;
	while(line = get_next_line(fd))
		ft_putstr(line);
}

Hint: Do not leak file descriptors! */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	ft_popen(const char *file, char *const av[], char type)
{
	if (!file || !av || (type != 'w' && type != 'r'))
		return (-1);
	int	fd[2];
	if (pipe(fd) < 0)
		return (-1);
	pid_t	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (dup2(fd[1], STDOUT_FILENO) < 0)
				exit (-1);
		}
		else
		{
			if (dup2(fd[0], STDIN_FILENO) < 0)
				exit (-1);
		}
		close(fd[0]);
		close(fd[1]);
		execvp(file, av);
		exit(-1);
	}
	if (type == 'r')
	{
		wait(NULL);
		close(fd[1]);
		return(fd[0]);
	}
	else
	{
		close(fd[0]);
		return(fd[1]);
	}

}

//test type 'r'
int main()
{
	//int fd = open("texte", O_RDONLY);
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	printf("File descriptor: %d", fd);
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
} 
/* 
//test type 'w'
int main()
{
    int fd = ft_popen("wc", (char *const[]){"wc", NULL}, 'w');
	if (fd == -1)
	{
		perror("error ft_popen args");
		return (0);
	}
	printf("File descriptor: %d\n", fd);
    char *input = "Hello world\nThis is a test\nthird line mofo\n";
    write(fd, input, strlen(input));
    close(fd);
	printf("after test\n");
    return (0);
} */
