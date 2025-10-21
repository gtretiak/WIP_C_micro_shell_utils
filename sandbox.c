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

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{}


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
