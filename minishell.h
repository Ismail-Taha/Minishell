/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:33:36 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:33:37 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern int					g_last_exit_status;

/**
 * Parser state for handling quotes
 */
typedef enum e_state
{
	STATE_NORMAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}							t_state;

/**
 * Token types for parsing
 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}							t_token_type;

/**
 * Token structure for lexical analysis
 */
typedef struct s_token
{
	char					*content;
	t_token_type			type;
	struct s_token			*next;
}							t_token;

/**
 * Redirection structure for I/O operations
 */
typedef struct s_redirections
{
	t_token_type			type;
	char					*file;
	int						heredoc_fd;
	int						was_in_squotes;
	int						was_in_dquotes;
	struct s_redirections	*next;
}							t_redirections;

/**
 * Command structure representing a single command or pipeline element
 */
typedef struct s_command
{
	char					**args;
	int						args_count;
	t_redirections			*redirections;
	struct s_command		*next;
}							t_command;

/**
 * Environment variable linked list structure
 */
typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

/* ===================== MEMORY MANAGEMENT ===================== */
void						free_command(t_command *cmd);
void						free_command_list(t_command *cmd_list);
void						safe_free(void **ptr);
void						safe_close(int *fd);
void						safe_doube_star_free(char **str);
void						free_env_list(t_env **env_list);

/* ===================== STRING UTILS ===================== */
int							is_number(char *str);
int							ft_isspace(char c);
char						*ft_strndup(const char *s, size_t n);
int							ft_strcmp(const char *s1, const char *s2);
int							ft_fprintf_fd(int fd, const char *format, ...);
char						*strip_quotes(const char *value);
int							is_content_quoted(char *content);

/* ===================== TOKENIZER ===================== */
typedef struct s_tokenizer
{
	t_token					**tokens;
	char					*input;
	int						i;
	int						start;
	t_state					state;
}							t_tokenizer;

typedef struct s_quote_ctx
{
	char					c;
	size_t					*i;
	int						*in_quotes;
	int						*quoted;
	char					*quote_type;
}							t_quote_ctx;

typedef struct s_strip_ctx
{
	int						in_quotes;
	char					quote_type;
	size_t					i;
	size_t					len_or_j;
	char					*result;
}							t_strip_ctx;

t_token						*tokenize_input(char *input);
void						assign_token_types(t_token *tokens);
int							is_operator(char c);
char						*extract_operator_token(char *str, int *pos);
char						*extract_word_token(char *str, int start, int end);
int							process_normal_char(t_tokenizer *t);
int							process_operator(t_tokenizer *t);
int							process_whitespace(t_tokenizer *t);
void						skip_whitespace(t_tokenizer *t);
int							process_end_of_input(t_tokenizer *t);
t_token						*clean_tokens_return_null(t_token **tokens);
void						handle_quotes(t_tokenizer *t);
int							add_token(t_token **tokens, char *content);

/* ===================== TOKEN LIST ===================== */
void						ft_token_add_back(t_token **lst, t_token *new);
void						ft_token_delone(t_token *lst, void (*del)(void *));
void						ft_token_clear(t_token **lst, void (*del)(void *));
t_token						*ft_token_new(char *content);
int							count_commands(t_command *cmd_list);

/* ===================== COMMAND CREATION & EXECUTION ===================== */
typedef struct s_cmd_ctx
{
	t_command				*cmd_list;
	int						pipe_fd[2];
	int						prev_pipe_read;
	int						status;
	t_command				*current;
	int						init_result;
	t_env					**env_list;
}							t_cmd_ctx;

t_command					*create_cmds(t_token **tokens);
int							execute_command_list(t_command *cmd_list,
								t_env **env_list);
char						*find_executable_path(char *cmd, char **envp);
int							setup_redirections(t_command *cmd);
int							setup_heredoc(char *delimiter, char **envp);
void						execute_single_command(t_cmd_ctx *cmd_ctx);
t_command					*create_command_type_word(t_token **tokens);
t_command					*command_init(void);
int							setup_redirect_in(char *file_path,
								int was_in_squotes, int was_in_dquotes);
int							setup_redirect_out(char *file_path, int append_mode,
								int was_in_squotes, int was_in_dquotes);
int							handle_redirect_token(t_token **current,
								t_command **first_cmd, t_command **current_cmd);
void						add_redirection(t_command *cmd,
								t_token_type redirect_type, char *file);
t_command					*finish_command_parsing(t_command *first_cmd);
/* Command redirection utilities */
int							count_word_tokens(t_token *current);
char						**allocate_args_array(t_command *cmd,
								int new_count);
int							add_word_to_args(t_token **current, int i,
								char **new_args);
void						add_words_as_args(t_command *cmd,
								t_token **current);

/* ===================== BUILTINS ===================== */
typedef struct s_env_setup
{
	char					**env_array;
	char					**temp_env;
	int						env_count;
	int						extra_vars;
	int						i;
	int						j;
}							t_env_setup;

int							is_builtin_command(char *cmd);
int							execute_builtin(t_cmd_ctx *cmd_ctx);
int							builtin_cd(t_command *cmd, t_env **env);
int							builtin_echo(t_command *cmd);
int							builtin_pwd(t_env **env);
int							builtin_export(t_command *cmd, t_env **env_list);
int							builtin_unset(t_command *cmd, t_env **env_list);
int							builtin_env(t_command *cmd, t_env *env_list);
int							builtin_exit(t_cmd_ctx *cmd_ctx);
int							export_one_arg(char *arg, t_env **env_list);
void						unset_one_arg(char *arg, t_env **env_list);
void						print_environment(t_env *env_list, t_command *cmd);
int							is_parent_builtin(char *cmd);
int							execute_single_parent_builtin(t_cmd_ctx *cmd_ctx);

/* ===================== ENV LINKED LIST HELPERS ===================== */
t_env						*find_env_node(t_env *env_list, const char *key);
void						add_or_update_env(t_env **env_list, const char *key,
								const char *value);
void						print_export_list(t_env *env_list);
t_env						*envp_to_env_list(char **envp);
char						**env_list_to_envp(t_env *env_list);
int							is_valid_identifier(char *str);

/* ===================== SIGNALS ===================== */
void						heredoc_sigint_handler(int sig);
void						sigint_handler(int sig);
void						ctrl_d_handle(void);
void						child_sigint_handler(int sig);
/* ===================== ENV EXPANSION UTILS ===================== */

typedef struct s_var_expand
{
	int						i;
	char					*result;
	int						handle_ret;
	int						is_empty_var;
	int						has_quoted_vars;
}							t_var_expand;

int							is_var_char(char c);
char						*extract_var_name(const char *str, int *pos);
char						*get_env_value(char *var_name, char **envp);
int							add_char_to_result(char **result, char c);
void						update_quote_state(char c, t_state *state);
char						*expand_variables(char *str, char **envp);
void						expand_command_args(t_command *cmd, char **envp);

/* ===================== ENV EXPANSION ARGS UTILS ===================== */
typedef struct s_expand_vars
{
	t_command				*cmd;
	char					**envp;
	int						i;
	int						j;
	int						added;
	int						is_export;
	int						was_arg_quoted;
	int						had_quoted_vars;
}							t_expand_vars;

int							count_split_words(char **split_words);
void						copy_and_replace_args(t_command *cmd,
								char **new_args, int pos, char **split_words);
int							add_split_args_to_command(t_command *cmd, int pos,
								char **split_words);
int							split_and_insert_args(t_expand_vars *v);
void						expand_args_loop(t_expand_vars *v);
void						expand_redirections_loop(t_expand_vars *v);
int							has_var_in_dquotes(const char *str);
int							is_var_in_squotes(const char *str);
int							was_quoted(const char *str);
void						clean_empty_args(t_command *cmd);

/* ===================== EXECUTOR UTILS ===================== */
int							setup_pipe(int pipe_fd[2]);
int							handle_heredoc_redir(t_redirections *redir,
								char **envp);
void						handle_fork_error(t_cmd_ctx *cmd_ctx);
void						handle_child_input(t_cmd_ctx *cmd_ctx);
void						handle_child_output(t_cmd_ctx *cmd_ctx);

/* ===================== EXECUTOR CHILD ===================== */
void						child_process(t_cmd_ctx *cmd_ctx);
int							wait_for_specific_pid(pid_t last_pid);
int							parent_process(int prev_pipe_read, int pipe_fd[2]);
int							setup_all_heredocs(t_command *cmd_list,
								char **envp);
/* ===================== UTILITY FUNCTIONS ===================== */
void						print_Cmd_list(t_command *cmd);
void						initialize_empty_env(t_env **env_list);
void						update_shlvl(t_env **env_list);

int							is_path_with_slash(char *cmd);
char						*check_direct_path(char *cmd);
#endif
