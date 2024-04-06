
include ../Makefile


$(ROOT_DIR)/tests/runner: $(SRC)/main.c $(objects)
	$(CC) -DMACHINE $(CFLAGS) $^ -o $@

clean:
	rm $(ROOT_DIR)/tests/runner

.DEFAULT_GOAL := $(ROOT_DIR)/tests/runner
