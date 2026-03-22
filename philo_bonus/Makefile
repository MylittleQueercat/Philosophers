NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

INC_DIR = ./
OBJ_DIR = obj/

SRC = main_bonus.c \
	utils_bonus.c  \
	action_bonus.c \
	init_bonus.c \
	print_bonus.c \
	process_bonus.c \
	moni_bonus.c \

INC = -I$(INC_DIR)
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo ""
	@echo "\033[1;30m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"
	@echo "                          🧘  \033[1;35mPhilosophers are seated at the table\033[0m"
	@echo ""
	@echo "                     \"We do not live to eat, but eat to live.\" - Socrates"
	@echo "\033[1;30m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"
	@echo ""

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@echo ""
	@echo "\033[1;30m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"
	@echo "\033[1;31m   🚶     🚶     🚶       The philosophers have left the table...\033[0m"
	@echo ""
	@echo "        \"Death is nothing to us, for when we exist death is not present.\" - Epicurus"
	@echo ""
	@echo "\033[2m                             [Silence fills the dining hall]\033[0m"
	@echo "\033[1;30m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m"
	@echo ""

re: fclean all

.PHONY: all clean fclean re
