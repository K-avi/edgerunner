TARGET := ../edgerunner
DEBUG := ../edgerunner_dbg

TEST := ../test
TEST_DBG := ../test_dbg

CC := gcc

DEBUG_FLAGS := -g -fsanitize=address -fsanitize=object-size -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=leak -fsanitize=alignment
CFLAGS := -O1 -g -std=gnu17 -Wall -Wextra -Wpedantic -Wno-unused-parameter -lcurses

SRCS := dynarr.c errflags.c graph_base.c graph_gen.c points.c graph_curses.c game.c player_curses.c ennemy.c search_utils.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET)
$(DEBUG) :   $(SRCS) main.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $(DEBUG) 


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) 
$(TEST_DBG): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $(TEST) 

