ROOT_DIR_ABS := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
ROOT_DIR := $(shell realpath --relative-to $(ROOT_DIR_ABS) $(CURDIR) )
BUILD    ?= $(ROOT_DIR)/build
SRC      := $(ROOT_DIR)/src

CFLAGS   := $(CFLAGS) -Wall -I$(SRC) -Werror -g -O3

sources  := $(wildcard $(SRC)/*.c)
sources  := $(filter-out %main.c, $(sources))

# Pega os nomes dos arquivos remove a extensão e diretório
objects  := $(notdir $(basename $(sources)))
# Concatena .o no final
objects  := $(addsuffix .o, $(objects))
# Contatena $BUILD/ no começo
objects  := $(addprefix $(BUILD)/, $(objects))

$(shell [[ -d "$(BUILD)" ]] || mkdir -p $(BUILD))

$(ROOT_DIR)/tp1: $(SRC)/main.c $(objects)
	$(CC) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
	rm tp1

.PHONY: clean

