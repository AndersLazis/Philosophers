# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/22 14:22:06 by aputiev           #+#    #+#              #
#    Updated: 2023/06/23 17:54:44 by aputiev          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address
RM			= /bin/rm -f

INCLUDE_DIR	= includes

BUILD_DIR	= build
SRC_DIR		= ./src
SRCS		=	main.c \
				check_args.c \
				finish_sim.c \
				init_data.c \
				routines.c \
				time.c
				
OBJS =	$(SRCS:%.c=$(BUILD_DIR)/%.o)

	BLACK        := $(shell tput -Txterm setaf 0)
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
	YELLOW       := $(shell tput -Txterm setaf 3)
	LIGHTPURPLE  := $(shell tput -Txterm setaf 4)
	PURPLE       := $(shell tput -Txterm setaf 5)
	BLUE         := $(shell tput -Txterm setaf 6)
	WHITE        := $(shell tput -Txterm setaf 7)
	RESET 		:= $(shell tput -Txterm sgr0)


all: $(NAME)
	@echo "${WHITE}===============================================================================================${RESET}"
	@echo "${GREEN}__________.__    .__.__                            .__                         	    		 ${RESET}"
	@echo "${GREEN}\______   \  |__ |__|  |   ____  __________ ______ |  |__   ___________  ______	    		 ${RESET}"
	@echo "${GREEN} |     ___/  |  \|  |  |  /  _ \/  ___/  _ \\____ \|  |  \_/ __ \_  __ \/  ___/	 			 ${RESET}"
	@echo "${GREEN} |    |   |   Y  \  |  |_(  <_> )___ (  <_> )  |_> >   Y  \  ___/|  | \/\___ \ 				 ${RESET}"
	@echo "${GREEN} |____|   |___|  /__|____/\____/____  >____/|   __/|___|  /\___  >__|  /____  >				 ${RESET}"
	@echo "${GREEN}               \/                   \/      |__|        \/     \/           \/		 ${RESET}"
	@echo "${WHITE}===============================================================================================${RESET}"
	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) -g $(CFLAGS) $(INCLUDES) -c $< -o $@

re: fclean all

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)
	
.PHONY:	all clean fclean re