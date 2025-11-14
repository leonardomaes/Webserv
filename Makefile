# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 15:52:11 by lmaes             #+#    #+#              #
#    Updated: 2025/11/14 19:00:42 by rda-cunh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compile Options
NAME = webserv
CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# Directories
SRC_DIR := src
INC_DIR := inc
BIN_DIR := bin

# Sources & Objects
SRC =	$(SRC_DIR)/main.cpp $(SRC_DIR)/Server.cpp $(SRC_DIR)/Config.cpp

OBJ = 	$(SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

VAL 			= valgrind --leak-check=full --show-leak-kinds=all \
					--track-fds=yes -s ./$(NAME)

# Commands
RM = rm -rf

# Rules
all: $(NAME)

$(NAME) : $(OBJ) | $(BIN_DIR)
	@$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR):
			mkdir -p $(BIN_DIR)

leaks: $(NAME)
		$(VAL)

clean:
			$(RM) $(OBJ) $(BIN_DIR)

fclean: clean
			$(RM) $(NAME)

re: fclean all
.PHONY: all leaks clean fclean re
