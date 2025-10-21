/*
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill,
				printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice, 0 if f is bad, or -1 in case of an error in
your function.

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exits with any other exit code than 0, or if it
times out.

If verbose is true, you must write the appropriate message among the following:

"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).

We will test your code with very bad functions.
*/

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void	do_nothing(int sig)
{
	(void)sig;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	struct sigaction sa = {0}; // to handle alarm signal, initialized to zeros
	int	status;
	pid_t	ret_pid;
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0) // the only thing the child does:
	{
		alarm(timeout);
		f();
		exit(0);
	}
	// the main job is supposed to be done by parent (as usual):
	sa.sa_handler = do_nothing; // replacement (if not set, sigalrm kills every process)
	sigemptyset(&sa.sa_mask); // we clear signal mask, to prevent other signals block or delay
	sa.sa_flags = 0; // since no one is killed yet, we reset flags (so waitpid wouldn't resume)
	if (sigaction(SIGALRM, &sa, NULL) < 0) // actual special instruction for SIGALRM
		return (-1);
	alarm(timeout); // We need to stop in case the child hangs
	ret_pid = waitpid(pid, &status, 0); // basically "wait", but tailored to a certain pid
	if (ret_pid < 0) // Child was interrupted by timeout alarm
	{
		if (errno == EINTR) //EINTR means interrupted by a signal
		{
			kill(pid, SIGKILL); // killing the poor guy
			waitpid(pid, NULL, 0); // we anyway collect the exit status of the child
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	alarm(0);// if child exits normally, we don't need an alarm anymore
	if (WIFEXITED(status)) // Child exited normally
	{
		int	code = WEXITSTATUS(status);
		if (code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function: exited with code %d\n", code);
		return (0);
	}
	if (WIFSIGNALED(status)) // Child was killed by a signal
	{
		int	sig = WTERMSIG(status);
		if (sig == SIGALRM)
		{
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
		}
		else
		{
			if (verbose)
				printf("Bad function: %s\n", strsignal(sig));
		}
		return (0);
	}
	struct sigaction reset = {0};
	reset.sa_handler = SIG_DFL;
	sigaction(SIGALRM, &reset, NULL);
	return (-1);
}


void	ok_f(void)
{
	printf("noice. ");
}

void	inf_f(void)
{
	while (1)
		;
}

void	bad_exit(void)
{
	exit(41);
}

void	suicide_f(void)
{
	kill(getpid(), SIGKILL);
	sleep(10);
}

void	fast_print_f(void)
{
	printf("fast_print_f executed\n");
}

void	abort_f(void)
{
	abort();
}

void	stop_f(void)
{
	raise(SIGSTOP);
	sleep(10);
}

void	segfault_f(void)
{
	int *p = NULL;
	*p = 42;
}

void	cancel_alarm(void)
{
	struct sigaction ca;
	ca.sa_handler = SIG_IGN;
	sigaction(SIGALRM, &ca, NULL);
	sleep(5);
	printf("f waited 5 sec, should be terminated before by alarm set in parent process if timeout < 5\n");
}

void	test_func(void (*f)(void), unsigned int timeout, const char* name)
{
	printf("==== Test : %s (timeout %us) ====\n", name, timeout);
	int res = sandbox(f, timeout, true);
	printf("Result = %d\n\n", res);
}

int	main(void)
{
	test_func(ok_f, 2, "ok_f (nice function, immediate return)");
	test_func(inf_f, 2, "inf_f (infinite loop, should timeout)");
	test_func(bad_exit, 2, "bad_exit (exit with code 41)");
	test_func(cancel_alarm, 2, "cancel_alarm (ignore SIGALRM, sleep 5s)");
	test_func(segfault_f, 2, "segfault_f (segmentation fault)");
	test_func(abort_f, 2, "abort_f (abort signal SIGABRT)");
	test_func(stop_f, 2, "stop_f (stops self with SIGSTOP, never continues)");
	test_func(suicide_f, 2, "suicide_f (kills self with SIGKILL)");
	test_func(fast_print_f, 2, "fast_print_f (quick print + return 0)");
	return 0;
}
