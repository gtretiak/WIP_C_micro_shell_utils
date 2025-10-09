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
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) < 0)
				exit (-1);
	//		close(fd[1]);
		}
		else
		{
			close(fd[1]);
			if (dup2(fd[1], STDIN_FILENO) < 0)
				exit (-1);
	//		close(fd[0]);
		}
		execvp(file, av);
		exit (-1);
	}
	else
		wait(NULL);
	if (type == 'r')
	{
		close(fd[1]);
		return(fd[0]);
	}
	else
	{
		close(fd[0]);
		return(fd[1]);
	}

}

int	main(void)
{
//	int	fd = ft_popen("ls", (char *const[]){"ls", "-l", NULL}, 'r');
	int	fd = ft_popen("./mini", (char *const[]){"./mini", NULL}, 'r');
	char	buf[1];
	printf("File descriptor: %d", fd);
	while (read(fd, buf, 1))
		write(1, buf, 1);
	close(fd);
	return (0);
}
