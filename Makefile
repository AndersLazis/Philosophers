NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror #-g -fsanitize=address
RM			= /bin/rm -f

INCLUDE_DIR	= includes
MLX_DIR		= mlx
MLX_FLAGS	= -L$(MLX_DIR) -lmlx \

BUILD_DIR	= build
SRC_DIR		= ./src
SRCS		=	main.c 
				
OBJS =	$(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(MLX_FLAGS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) -g $(CFLAGS) $(INCLUDES) -c $< -o $@

re: fclean all

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)
	
.PHONY:	all clean fclean re