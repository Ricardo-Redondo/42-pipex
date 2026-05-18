NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincludes

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS_DIR = srcs/
OBJS_DIR = objs

SRCS_FILES = pipex.c utils.c
BONUS_SRCS_FILES = pipex_bonus.c utils_bonus.c list_utils_bonus.c utils.c

OBJS = ${addprefix ${OBJS_DIR}/, ${SRCS_FILES:.c=.o}}
BONUS_OBJS = ${addprefix ${OBJS_DIR}/, ${BONUS_SRCS_FILES:.c=.o}}

# ===== RULES =====

all: $(NAME)
	@clear
	@printf "$$HEADER\n"

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

$(OBJS_DIR)/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

bonus: $(BONUS_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft
	@clear
	@printf "$$BONUS_H\n"

create:
	@touch file_in.txt file_out.txt
	@echo "created input and output file."

delete:
	@rm -f file_in.txt file_out.txt
	@if [ -z "$(filter clean fclean re, $(MAKECMDGOALS))" ]; then \
		echo "deleted input and output file."; \
	fi

clean: delete
	@rm -rf $(OBJS_DIR)
	@make -s -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@rm -rf $(OBJS_DIR)
	@make -s -C $(LIBFT_DIR) fclean

re:
	$(MAKE) fclean
	$(MAKE) all

rebonus:
	$(MAKE) fclean
	$(MAKE) bonus

.PHONY: all bonus clean fclean re rebonus create delete

define HEADER
╔════════════════════════════════╗
║         PIPEX  COMPILED        ║
╚════════════════════════════════╝
endef
export HEADER

define BONUS_H
╔════════════════════════════════╗
║         PIPEX  BONUS           ║
║           COMPILED             ║
╚════════════════════════════════╝
endef
export BONUS_H