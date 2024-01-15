# Makefile for webserv
# ...
NAME 		:= webserv

CC			:= c++

FLAGS 		:= -Wall -Wextra -Werror --std=c++98 

INC 		:= -I include/

#HOME SRCS LIST
HSL			:= main.cpp
#HOME SRCS PATH
HSP			:= $(addprefix srcs/home/,$(HSL))
#CORE SRCS LIST
CSL			:= Lexer.cpp Config.cpp Server.cpp Location.cpp Utils.cpp Http.cpp Logger.cpp Client.cpp Request.cpp RequestParser.cpp Response.cpp Cgi.cpp
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
	@mkdir -p $(O_DIR)

$(O_DIR)%.o : srcs/home/%.cpp
	@echo "$(GREEN)Compiling $@"
	@$(CC) $(FLAGS) $(INC) -c $< -o $@
	@sleep .1

$(O_DIR)%.o : srcs/core/%.cpp
	@echo "Compiling $(GREEN)$@"
	@$(CC) $(FLAGS) $(INC) -c $< -o $@
	@sleep .1

clean:
	@rm -rf $(O_DIR)

fclean: clean
	@rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re