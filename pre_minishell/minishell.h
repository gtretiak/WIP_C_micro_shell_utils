/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:23:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 19:02:40 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./Libft/libft.h"
# include <signal.h>

//Colors
# define GREEN "\001\033[32m\002"
# define RESET "\001\033[0m\002"

typedef struct s_tokens
{
	char			*line;
	struct s_tokens	*prev;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_env
{
	char			*var;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	char		*cwd;
	char		*username;
	char		*session;
	char		*hostname;
	char		*fullname;
	char		*prompt;
	char		*color_start;
	char		*color_end;
	char		*input;
	t_tokens	*tokens;
	t_env		*env;
	char		*bin_dir;
}	t_mini;

//Settings
void	ft_welcoming(void);
void	ft_init_minishell(t_mini *ms);
void	ft_get_full_name(t_mini *ms);
void	ft_get_env(t_mini *ms, char **env);

//Prompt settings
char	*ft_make_prompt(t_mini *ms);
char	*ft_get_fancy_address(t_mini *ms);
char	*ft_finish_prompt(t_mini *ms, char *cwd);
char	*ft_add_colors(t_mini *ms, char *prompt);

//Signals
void	ft_setup_signals(void);
void	ft_handle_ctrl_c(int signal);

//Tokenization
void	ft_tokenization(t_mini *ms);
int		ft_append_token(t_tokens **tokens, char *s);

//Cleaning up
void	ft_clean_str(t_mini *ms, int code);
void	ft_clean_cwd(t_mini *ms);
void	ft_clean_name(t_mini *ms);
void	ft_free_lists(t_tokens **tokens);

//Built-in functions
void	ft_exit(t_tokens *exit_node);
void	ft_pwd(t_mini *ms);
#endif
