TARGET := ../edgerunner
DEBUG := ../edgerunner_dbg

TEST := ../test
TEST_DBG := ../test_dbg

CC := gcc

DEBUG_FLAGS := -g -fsanitize=address -fsanitize=object-size -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=leak -fsanitize=alignment
CFLAGS := -Og -std=gnu17 -Wall -Wextra -Wpedantic -Wno-unused-parameter

SRCS := dynarr.c errflags.c graph_base.c graph_gen.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET)
$(DEBUG) :   $(SRCS) main.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $(DEBUG) 


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) 
$(TEST_DBG): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $(TEST) 

