SRCS	=	./source/exifsec.c \
				./source/utils.c

OBJS	=	$(SRCS:.c=.o)

HEADS	=	./includes

NAME	=	exifsec

GCC		=	gcc

FLAGS	=	-Wall -Werror -Wextra  -fsanitize=address

RM		=	rm -f

%.o: %.c ./include/exifsec.h
	$(GCC) $(FLAGS) -c $< -o $@ 

$(NAME): $(OBJS) 
	$(GCC) $(FLAGS) $(OBJS) -lexif -o $(NAME)

all:	$(NAME)

clean:
		$(RM) $(OBJS)

fclean: clean
		$(RM) $(NAME)

re:		fclean all
