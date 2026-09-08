NAME = ft_ping

SRC_DIR = src/
OBJ_DIR = obj/
HDR_DIR = includes/

COMP = cc
CFLAGS = -Wall -Wextra -Werror

SRC = $(shell find $(SRC_DIR) -name '*.c')

OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

DEP = $(OBJ:%.o=%.d)

DEL = $(shell printf "\r\033[K")
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[0;33m
NOCOL = \033[m
BOLD = $(shell tput bold)
NOBOLD = $(shell tput sgr0)

MSG_CLEANING = "$(RED)$(BOLD)cleaning $(NAME)...$(NOBOLD)$(NOCOL)";
MSG_CLEANED = "$(RED)$(BOLD)cleaning done$(NOBOLD)$(NO_COLOR)";
MSG_COMPILING = "$(YELLOW)$(BOLD)compiling:$(NOBOLD)$(NOCOLOR) $(<)...";
MSG_READY = "$(BLUE)$(BOLD)$(NAME) ready$(NOBOLD)$(NOCOLOR)";


all : $(NAME)

$(NAME) : $(OBJ)
	@$(COMP) $(CFLAGS) $(OBJ) -I $(HDR_DIR) -o $(NAME)
	@echo -n $(DEL)
	@echo $(MSG_READY)

-include $(DEP)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@ mkdir -p $(@D)
	@echo -n $(DEL)
	@echo -n $(MSG_COMPILING)
	@ $(COMP) $(CFLAGS) -MMD -c $< -o $@ -I $(HDR_DIR)

clean :
	@echo $(MSG_CLEANING)
	@rm -rf $(OBJ_DIR)
	@echo $(MSG_CLEANED)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re 
