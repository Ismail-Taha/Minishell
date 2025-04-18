#ifndef MINISHELL_H
# define MINISHELL_H

# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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
	STATE_NORMAL,          // Normal parsing state
	STATE_IN_SINGLE_QUOTE, // Inside single quotes
	STATE_IN_DOUBLE_QUOTE  // Inside double quotes
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
	char *content;        // Token content
	t_token_type type;    // Token type
	struct s_token *next; // Next token in list
}							t_token;

/**
 * Redirection structure for I/O operations
 */
typedef struct s_redirections
{
	t_token_type			type;
	char					*file;
	int						heredoc_fd;
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

/* ===================== STRING UTILS ===================== */
int							is_number(char *str);
int							ft_isspace(char c);
char						*ft_strndup(const char *s, size_t n);
int							ft_strcmp(const char *s1, const char *s2);

/* ===================== TOKENIZER ===================== */
t_token						*tokenize_input(char *input);
void						expand_command_args(t_command *cmd, char **envp);
void						assign_token_types(t_token *tokens);
int							is_operator(char c);
char						*extract_operator_token(char *str, int *pos);
char						*extract_word_token(char *str, int start, int end);
int							process_normal_char(t_token **tokens, char *input,
								int *i, int *start);
int							process_operator(t_token **tokens, char *input,
								int *i, int *start);
int							process_whitespace(t_token **tokens, char *input,
								int *i, int *start);
void						skip_whitespace(char *input, int *i, int *start);
int							process_end_of_input(t_token **tokens, char *input,
								int i, int start, t_state state);
t_token						*clean_tokens_return_null(t_token **tokens);
void						handle_quotes(char *input, int *i, t_state *state);
int							add_token(t_token **tokens, char *content);

/* ===================== TOKEN LIST ===================== */
void						ft_token_add_back(t_token **lst, t_token *new);
void						ft_token_delone(t_token *lst, void (*del)(void *));
void						ft_token_clear(t_token **lst, void (*del)(void *));
t_token						*ft_token_new(char *content);

/* ===================== COMMAND CREATION & EXECUTION ===================== */
t_command					*create_cmds(t_token **tokens);
int							execute_command_list(t_command *cmd_list,
								t_env **env_list);
char						*find_executable_path(char *cmd, char **envp);
int							setup_redirections(t_command *cmd);
int							setup_heredoc(char *delimiter);
void						execute_single_command(t_command *current,
								t_env *env_list);
t_command					*create_command_type_word(t_token **tokens);
t_command					*command_init(void);
int							setup_redirect_in(char *file_path);
int							setup_redirect_out(char *file_path,
								int append_mode);
int							handle_redirect_token(t_token **current,
								t_command **first_cmd, t_command **current_cmd);

/* ===================== BUILTINS ===================== */
int							is_builtin_command(char *cmd);
int							execute_builtin(t_command *cmd, t_env **env_list);
int							builtin_cd(t_command *cmd);
int							builtin_echo(t_command *cmd);
int							builtin_pwd(void);
int							builtin_export(t_command *cmd, t_env **env_list);
int							builtin_unset(t_command *cmd, t_env **env_list);
int							builtin_env(t_env *env_list);
int							builtin_exit(t_command *cmd);
void						export_one_arg(char *arg, t_env **env_list);
void						unset_one_arg(char *arg, t_env **env_list);

/* ===================== ENV LINKED LIST HELPERS ===================== */
t_env						*find_env_node(t_env *env_list, const char *key);
t_env						*add_or_update_env(t_env **env_list,
								const char *key, const char *value);
void						print_export_list(t_env *env_list);
t_env						*envp_to_env_list(char **envp);
char						**env_list_to_envp(t_env *env_list);
int							is_valid_identifier(char *str);

/* ===================== SIGNALS ===================== */
void						setup_signals(void);

/* ===================== ENV EXPANSION UTILS ===================== */
int							is_var_char(char c);
char						*extract_var_name(const char *str, int *pos);
char						*get_env_value(char *var_name, char **envp);
int							add_char_to_result(char **result, char c);
void						update_quote_state(char c, t_state *state);
char						*expand_variables(char *str, char **envp);
char						*strip_quotes(const char *value);

/* ===================== EXECUTOR UTILS ===================== */
int							setup_pipe(int pipe_fd[2]);
int							handle_heredoc_redir(t_redirections *redir);
void						handle_fork_error(t_command *current,
								int prev_pipe_read, int pipe_fd[2]);

/* ===================== EXECUTOR CHILD ===================== */
void						child_process(t_command *current,
								int prev_pipe_read, int pipe_fd[2],
								t_env *env_list);
int							wait_for_children(void);
int							parent_process(int prev_pipe_read, int pipe_fd[2]);

/* ===================== EXECUTOR EXEC ===================== */
int							setup_command_pipe(t_command *current,
								int *prev_pipe_read, int pipe_fd[2]);
int							is_parent_builtin(char *cmd);
int							print_env_vars(char **envp);
void						handle_builtin_command(t_command *current,
								t_env *env_list);
void						handle_external_command(t_command *current,
								t_env *env_list);

#endif