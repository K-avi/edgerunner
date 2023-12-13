TARGET:= edgerunner
TEST := test
TEST_DBG := test_dbg
DEBUG := edgerunner_dbg

ALLPROGS= $(TARGET) $(TEST) $(DEBUG)

.PHONY:	all clean

all: $(ALLPROGS)
progs: $(PROGRAMS)

$(TEST): 
	$(MAKE) -C src/ ../test

$(TEST_DBG): 
	$(MAKE) -C src/ ../test_dbg

$(TARGET): 
	$(MAKE) -C src/ ../edgerunner

$(DEBUG): 
	$(MAKE) -C src/ ../edgerunner_dbg
clean:

	rm -f $(ALLPROGS)

.PHONY: clean
