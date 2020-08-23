# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: coremart <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/23 11:24:18 by coremart          #+#    #+#              #
#    Updated: 2020/08/23 11:55:37 by coremart         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

##COMPILATION ##
NAME = philo
ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security \
			-fsanitize=undefined
CFLAGS = -g -Werror -Wall -Wextra -std=c99 -pthread
DFLAGS = -MT $@ -MMD -MP -MF $(DDIR)/$*.d
AFLAGS =
ASAN =

## INCLUDES ##
LIB = libft
LIBH = $(LIB)/include
HDIR = include
LIBA = $(LIB)/libft.a

## SOURCES ##
SDIR = src
_SRCS = main.c state.c window.c mutex_io.c fill_tex_info.c coord.c
SRCS = $(patsubst %,$(SDIR)/%,$(_SRCS))

## OBJECTS ##
ODIR = obj
_OBJS = $(_SRCS:.c=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

## DEPENDENCIES ##
DDIR = deps
_DEPS = $(_OBJS:.o=.d)
DEPS = $(patsubst %,$(DDIR)/%,$(_DEPS))

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	@if [ "$(AFLAGS)" == "" ];\
	then\
		make -j 8 -C $(LIB);\
	else\
		make -j 8 -C $(LIB) asan;\
	fi
	clang -o $(NAME) $(LIBA) $(OBJS) $(CFLAGS) $(AFLAGS) -L./glfw -lglfw3 -L./glad -lglad -framework Cocoa -framework IOkit -framework CoreFoundation -framework OpenGl -framework Quartz

$(ODIR)/%.o: $(SDIR)/%.c
	clang $(CFLAGS) $(DFLAGS) -o $@ -c $< -I $(HDIR) -I glfw/include -I glad/include -I stb $(AFLAGS)

-include $(DEPS)

clean:
	@make -j 8 -C $(LIB) clean
	@rm -f $(OBJS)

fclean: clean
	@make -j 8 -C $(LIB) fclean
	@rm -f $(NAME)
	@rm -rf $(NAME).dSYM

re: fclean all

asan: AFLAGS = $(ASANFLAGS)
asan: all

reasan: AFLAGS = $(ASANFLAGS)
reasan: re

.PRECIOUS: $(DDIR)/%.d
.PHONY: all clean fclean re $(NAME)
