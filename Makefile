# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdaudre- <fdaudre-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/03/18 15:06:57 by fdaudre-          #+#    #+#              #
#    Updated: 2015/03/18 17:09:26 by alelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#################
##  VARIABLES  ##
#################

#	Sources
SRCDIR		=	src
SRC			=	main.c					\
				get_env.c				\
				set_env.c				\
				is_dir.c				\
				build_env.c				\
				ft_env.c				\
				ft_exebin.c				\
				ft_builtins.c			\
				ft_exit.c				\
				ft_unsetenv.c			\
				ft_setenv.c				\
				ft_echo.c				\
				ft_cd.c					\
				unset_env.c				\
				ft_signals.c			\
				prompt.c				\
				default_terminal_mode.c	\
				raw_terminal_mode.c		\
				prompt/get_command.c	\

#	Objects
OBJDIR		=	obj

#	Includes
INCDIR		=	. libft

#	Libraries
LIBDIR		=	libft
LIB			=	-lft -lncurses

#	Output
NAME		=	42sh

#	Compiler
CFLAGS		=	-g -Werror -Wall -Wextra
CC			=	clang

#################
##  AUTO       ##
#################

SHELL		=	/bin/zsh
OBJ			=	$(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
LIBFLAG		=	$(addprefix -L,$(LIBDIR))
INCFLAG		=	$(addprefix -I,$(INCDIR))
NORME		=	$(addsuffix /*.h,$(INCDIR)) $(addprefix $(SRCDIR)/,$(SRC))

$(foreach S,$(SRC), \
	$(eval VPATH += $(SRCDIR)/$(dir $(S)) \
	) \
	)

#################
##  TARGETS    ##
#################

#	First target
all: $(NAME)

#	Linking
$(NAME): $(OBJ)
	@make -C libft
	@echo "\033[38;5;149m⇒ Linking:"
	@echo "\033[38;5;148m⇒ \033[38;5;149m"$(CC) $(CFLAGS) $(LIBFLAG) -o $(NAME) $(LIB) $(OBJ)"\033[31m"
	@$(CC) $(CFLAGS) $(LIBFLAG) -o $(NAME) $(LIB) $(OBJ)

#	Objects compilation
$(OBJDIR)/%.o: %.c
	@if [ $(ALREADY_OBJ)x != xx ]; then \
		echo "\033[38;5;135mBuilding:"; \
		fi
	@mkdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@echo "\033[38;5;123m⇒ \033[38;5;159m"$(CC) $(CFLAGS) $(INCFLAG) -o $@ -c $<"\033[31m"
	@$(CC) $(CFLAGS) $(INCFLAG) -o $@ -c $<
	$(eval ALREADY_OBJ=x)

#	Removing objects
clean:
	@if [ $(ALREADY_RM)x != xx ]; then \
		echo "\033[38;5;53mCleaning:"; \
		fi
	@printf "\033[38;5;52m⇒ \033[38;5;53m"
	rm -fv $(OBJ)
	@rmdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@printf "\033[31m"
	$(eval ALREADY_RM=x)

#	Removing objects and exe
fclean: clean
	@if [ $(ALREADY_RM)x != xx ]; then \
		echo "\033[38;5;53mCleaning:"; \
		fi
	@printf "\033[38;5;52m⇒ \033[38;5;53m"
	rm -fv $(NAME)
	@printf "\033[31m"
	$(eval ALREADY_RM=x)

#	All removing then compiling
re: fclean all

#	Checking norme
norme:
	@echo "\033[38;5;47mNorme:"
	@norminette $(NORME)\
		| sed "s/Norme/[0;1;37m➤ [38;5;47mNorme/g;s/Warning/[0;31mWarning/g;s/Error/[0;31mError/g"

run: $(NAME)
	@printf "\033[0m"
	./$(NAME)

codesize:
	@cat $(SRC) |grep -v '/\*' |wc -l

.PHONY: all clean fclean re norme codesize
