# Makefile for webserv
# ...
NAME 		:= webserv

CC			:= c++

FLAGS 		:= -Wall -Wextra -Werror --std=c++98 #-fsanitize=address

INC 		:= -I include/

#HOME SRCS LIST
HSL			:= main.cpp
#HOME SRCS PATH
HSP			:= $(addprefix srcs/home/,$(HSL))
#CORE SRCS LIST
CSL			:= lexer.cpp config.cpp server.cpp
#CORE SRCS PATH
CSP			:= $(addprefix srcs/core/,$(CSL))

#SOURCES LIST WITHOUT PATH
SL			:= $(HSL) $(CSL)
#SOURCES WITH PATH
SRCS		:= $(HSP) $(CSP)

#Object
O_DIR  		= object/
O_LIST      = $(patsubst %.cpp, %.o, $(SL))
OBJ         = $(addprefix $(O_DIR), $(O_LIST))

# COLORS
GREEN               = \033[0;32m
RED                 = \033[0;31m
RESET               = \033[0m

all : $(NAME)

$(NAME): $(O_DIR) $(OBJ)
	@tput el
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "$(RESET)\n$(NAME): $(GREEN)$(NAME) object files was created"
	@echo "$(RESET)$(NAME): $(GREEN)$(NAME) was created"

$(O_DIR):
	mkdir -p $(O_DIR)

$(O_DIR)%.o : srcs/home/%.cpp
	@tput cuu1
	@$(CC) $(FLAGS) $(INC) -c $< -o $@
	@echo "Compiling $(GREEN)$@"

$(O_DIR)%.o : srcs/core/%.cpp
	@tput cuu1
	@$(CC) $(FLAGS) $(INC) -c $< -o $@
	@echo "Compiling $(GREEN)$@"

clean:
	@rm -rf $(O_DIR)

fclean: clean
	@rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re