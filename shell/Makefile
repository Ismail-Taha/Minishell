NAME = minishell

LIBFT_DIR = Libft
GNL_DIR = get_next_line
LIBFT = $(LIBFT_DIR)/libft.a
GNL = $(GNL_DIR)/gnl.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(GNL_DIR) 
LIBS = $(LIBFT) $(GNL) 
INCLUDES = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lhistory

SRCS =\
	main.c \
	utils.c \
	signals.c \
	tokenizer.c \
	token_utils.c \
	token_types.c \
	lst_functions.c \
	command_path.c \
	command_init.c \
	command_redirection.c \
	command_parser.c \
	heredoc.c \
	executor_main.c \
	executor_exec.c \
	executor_child.c \
	executor_utils.c \
	builtins_cd_pwd.c \
	builtins_echo.c \
	builtins_exit.c \
	memory.c \
	env_expand_utils1.c \
	env_expand_utils2.c \
	builtins_env_unset_export.c \
	builtins_env_export_utils.c \
	builtins_export_utils.c \
	builtins_unset_utils.c \
	env_expand_main.c

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(GNL) $(NAME)

$(GNL):
	@make -C $(GNL_DIR)

$(LIBFT):
	@make -C $(LIBFT_DIR) all bonus

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -f $(LIBFT) ]; then make -C $(LIBFT_DIR) clean; fi
	@if [ -f $(GNL) ]; then make -C $(GNL_DIR) clean; fi
fclean: clean
	rm -f $(NAME)
	@if [ -f $(LIBFT) ]; then make -C $(LIBFT_DIR) fclean; fi
	@if [ -f $(GNL) ]; then make -C $(GNL_DIR) fclean; fi

re: fclean all

.PHONY: all clean fclean re
