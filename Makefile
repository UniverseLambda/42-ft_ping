.PHONY: all clean fclean re prod_reminder

NAME := ft_ping

BUILD_TYPE := DEBUG

SRC_FILES := main.c cli.c ft_util.c ft_result.c ft_string.c
INC_FILES := cli.h ft_util.h ft_result.h ft_string.h

SRCS_DIR := src
INCS_DIR := inc
OBJS_DIR := .objs

SRCS := $(addprefix $(SRCS_DIR)/, $(SRC_FILES))
INCS := $(addprefix $(SRCS_DIR)/$(INCS_DIR)/, $(INC_FILES))
OBJS :=	$(addprefix $(OBJS_DIR)/, $(SRC_FILES:.c=.o))

CC := gcc
LD := gcc
RM := rm
MKDIR := mkdir

CFLAGS := $(CFLAGS) -g3 -Wall -Wextra -Werror -std=c99 #-fsanitize=address -fsanitize=undefined
LDFLAGS := $(LDFLAGS) -g3 -Wall -Wextra -Werror -std=c99 #-fsanitize=address -fsanitize=undefined

CFLAGS += -D$(BUILD_TYPE)=

all: $(NAME)

$(NAME):	$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)  $(BUILD_OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c Makefile $(INCS)
	@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) -rf $(OBJS_DIR)

fclean: clean
	$(RM) -rf $(NAME)

re: fclean all

prod_reminder:
	@echo "Need to change BUILD_TYPE to PROD"
