.PHONY: all clean fclean re prod_reminder

NAME := ft_ping

BUILD_TYPE := PROD

SRC_FILES := cli.c dump.c ft_bits.c ft_result.c ft_signal.c ft_sqrt.c ft_string.c ft_time.c ft_util.c get_error.c init.c iter.c main.c misc.c packet_util.c ping_stats.c print.c
INC_FILES := cli.h dump.h ft_bits.h ft_result.h ft_signal.h ft_sqrt.h ft_string.h ft_time.h ft_util.h get_error.h init.h iter.h misc.h packet_util.h ping_stats.h print.h

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

CFLAGS := $(CFLAGS) -g3 -Wall -Wextra -Werror -std=gnu89 -fsanitize=address -fsanitize=undefined -fstack-protector-all
LDFLAGS := $(LDFLAGS) -g3 -Wall -Wextra -Werror -std=gnu89 -fsanitize=address -fsanitize=undefined -fstack-protector-all

CFLAGS += -D$(BUILD_TYPE)=

all: $(NAME)

$(NAME):	$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(BUILD_OBJS)

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
	@echo "Re-check headers and sources listing in Makefile"
	@echo "Launch the norm"
	@echo "Remove the -fsanitize=undefined"
