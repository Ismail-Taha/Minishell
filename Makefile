NAME = minishell

# Directory structure
LIBFT_DIR = libft
SRC_DIR = .
BUILTINS_DIR = builtins
TOKENIZER_DIR = tokenizer
PARSER_DIR = parser
EXECUTOR_DIR = executor
ENV_DIR = env
UTILS_DIR = utils
SIGNALS_DIR = signals

# Build directory for objects and dependencies
BUILD_DIR = build

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = $(LIBFT)

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(LIBFT_DIR) -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lhistory 

# Source files by module
MAIN_SRCS = main.c
UTILS_SRCS = $(addprefix $(UTILS_DIR)/, string_utils.c memory_utils.c main_utils.c fprintf_utils.c quote_utils.c quote_check_utils.c)
TOKENIZER_SRCS = $(addprefix $(TOKENIZER_DIR)/, tokenizer.c token_utils.c token_types.c token_list_utils.c tokenizer_process.c)
PARSER_SRCS = $(addprefix $(PARSER_DIR)/, command_path.c command_init.c command_redirection.c command_parser.c command_redirect_setup.c command_redirection_utils.c)
EXECUTOR_SRCS = $(addprefix $(EXECUTOR_DIR)/, heredoc.c executor_main.c executor_exec.c executor_child.c executor_utils.c executor_builtin.c)
BUILTINS_SRCS = $(addprefix $(BUILTINS_DIR)/, builtins_cd.c builtins_pwd.c builtins_echo.c builtins_exit.c builtins_env.c builtins_env_export_utils.c builtins_export.c builtins_unset.c builtins_env_print.c builtins_export_utils.c builtins_env_convert_utils.c)
ENV_SRCS = $(addprefix $(ENV_DIR)/, env_expansion.c env_variable_expand.c env_quote_handling.c env_arg_processing.c env_variable_process.c env_arg_utils.c)
SIGNALS_SRCS = $(addprefix $(SIGNALS_DIR)/, signals.c)

# All source files
SRCS = $(MAIN_SRCS) $(UTILS_SRCS) $(TOKENIZER_SRCS) $(PARSER_SRCS) $(EXECUTOR_SRCS) $(BUILTINS_SRCS) $(ENV_SRCS) $(SIGNALS_SRCS)

# Object files with build directory
OBJS = $(SRCS:%.c=%.o)

# Dependency files
DEPS = $(OBJS:.o=.d)


all: $(LIBFT) $(NAME) 
	@make clean

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(UTILS_DIR)
	mkdir -p $(BUILD_DIR)/$(TOKENIZER_DIR)
	mkdir -p $(BUILD_DIR)/$(PARSER_DIR)
	mkdir -p $(BUILD_DIR)/$(EXECUTOR_DIR)
	mkdir -p $(BUILD_DIR)/$(BUILTINS_DIR)
	mkdir -p $(BUILD_DIR)/$(ENV_DIR)
	mkdir -p $(BUILD_DIR)/$(SIGNALS_DIR)

# Compile external libraries
$(LIBFT):
	@make -C $(LIBFT_DIR) all bonus

# Link the final executable
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@echo "$(NAME) successfully built!"

# Compile C files to object files with dependencies
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean rules
clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)
	@if [ -f $(LIBFT) ]; then make -C $(LIBFT_DIR) fclean; fi
	@echo "All build artifacts cleaned."

re: fclean all


.SECONDARY: $(OBJS)
.PHONY: all clean fclean re
