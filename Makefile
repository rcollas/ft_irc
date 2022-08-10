NAME		=	ft_irc

SRC_FILES 	=	main.cpp \
				Parser.cpp \
				Server/Server.cpp \
				Server/NumericReplies.cpp \
				Server/Channel.cpp \
				Server/ChannelServer.cpp \
				Server/Command.cpp \
				User/User.cpp \

OBJS_DIR	=	objs

SRC_DIR		=	./srcs/

INC_DIR		=	./incs/

INC_FILES	=	Server/Server.hpp \
				Server/NumericReplies.hpp \
				Server/Channel.hpp \
				User/User.hpp \

OBJS		=	$(addprefix $(OBJS_DIR)/, $(SRC_FILES:.cpp=.o))

INCLUDE		=	$(addprefix $(INC_DIR)/, $(INC_FILES))

CC			=	clang++

CFLAGS		=	-Wall -Wextra -Werror -g -std=c++98

RM			=	rm -rf

RESET		=	\033[0m
RED			=	\033[1;31m
GREEN		=	\033[1;32m
YELLOW		=	\033[1;33m
BLUE		=	\033[1;34m
WHITE		=	\033[1;37m
ORANGE		=	\033[1;38;5;208m
UP			=	\033[A
CUT			=	\033[K

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE)
				@mkdir -p $(@D)
				@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
				@echo "$(CUT)$(BLUE)clang $(RESET)$(notdir $@)"
				@printf "$(UP)"

all:	$(NAME)

$(NAME):		$(OBJS)
				@$(CC) $(CFLAGS) $^ -o $@
				@echo "$(CUT)$(GREEN)✔ $(NAME) created$(RESET)"

fclean:	clean
				@$(RM) $(NAME)
				@echo "$(RED)✘ fclean$(RESET)"


clean:
				@$(RM) $(OBJS) $(OBJS_DIR)
				@echo "$(RED)✘ clean$(RESET)"

re:		fclean all

.PHONY:	all clean fclean re