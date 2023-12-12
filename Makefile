# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yitoh <yitoh@student.codam.nl>               +#+                      #
#                                                    +#+                       #
#    Created: 2023/10/31 19:35:41 by yitoh         #+#    #+#                  #
#    Updated: 2023/12/12 20:59:16 by yitoh         ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=philo
CC		=cc
CFLAGS	=-Wall -Werror -Wextra #-g -fsanitize=thread #-flto -O3
RM		=rm -f
SRC		=src/main.c\
		 src/init.c\
		 src/philo_action.c\
		 src/monitor.c\
		 src/cleanup.c\
		 src/tool.c
OBJ		=$(SRC:src/%.c=obj/%.o)

all:	$(NAME)

clean:
				@$(RM) -r obj
				@printf "\e[1;35mCleaned Object Files\n\e[0;00m"

fclean:		clean
				@$(RM) $(NAME)
				@printf "\e[1;31mCleaned Executables\n\e[0;00m"

re:			fclean all

$(OBJ):		$(SRC) philosophers.h Makefile
				@mkdir -p $(dir $@)
				@printf "\e[1;34mBuilding $@\n\e[0;00m"
				@$(CC) $(CFLAGS) -c $(@:obj/%.o=src/%.c) -o $@

$(NAME):	$(OBJ)
				@printf "\e[1;36mCompiling $@\n\e[0;00m"
				@$(CC) $(CFLAGS) $^ -o $@
				@printf "\e[1;32mDone\e[0;00m\n"


.PHONY:		all clean fclean re libft
