NAME := pipex
CC := cc
INCDIR := ./
C_FLAGS := -Wall -Wextra -Werror 
LIBFT_DIR := libft/
LIBFT_ARCH := $(addsuffix libft.a, $(LIBFT_DIR))

INC_FLAGS :=  -I$(INCDIR) -I$(LIBFT_DIR)

FILES = pipex.c 

SRCS := main.c $(addprefix src/, $(FILES))
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