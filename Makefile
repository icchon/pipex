NAME := pipex
CC := cc
INCDIR := ./includes/
C_FLAGS := -Wall -Wextra -Werror 
LIBFT_DIR := libft/
LIBFT_ARCH := $(addsuffix libft.a, $(LIBFT_DIR))

INC_FLAGS :=  -I$(INCDIR) -I$(LIBFT_DIR)

EXECUTER_FILES := executer.c
EXECUTER_SRCS := $(addprefix executer/, $(EXECUTER_FILES))

LEXER_FILES := lexer.c token.c 
LEXER_SRCS := $(addprefix lexer/, $(LEXER_FILES))

PARSER_FILES := parser.c
PARSER_SRCS := $(addprefix parser/, $(PARSER_FILES))

TRANSLATER_FILES := translater.c
TRANSLATER_SRCS := $(addprefix translater/, $(TRANSLATER_FILES))

EXPANDER_FILES := expander.c
EXPANDER_SRCS := $(addprefix expander/, $(EXPANDER_FILES))

UTILS_FILES := free.c 
UTILS_SRCS := $(addprefix utils/, $(UTILS_FILES))

PROCESS_FILES := process.c
PROCESS_SRCS := $(addprefix process/, $(PROCESS_FILES))

FILES := main.c $(PROCESS_SRCS) $(EXPANDER_SRCS) $(EXECUTER_SRCS) $(LEXER_SRCS) $(PARSER_SRCS) $(TRANSLATER_SRCS) $(UTILS_SRCS)
SRCS := $(addprefix src/, $(FILES))
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_ARCH)
	$(CC) $(C_FLAGS) $(INC_FLAGS) $(OBJS) $(LIBFT_ARCH) -o $@

$(LIBFT_ARCH):
	@make -C $(LIBFT_DIR) all

.c.o: 
	$(CC) $(C_FLAGS) $(INC_FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) 
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(LIBFT_ARCH)

re: fclean all

.PHONY: all clean fclean re 