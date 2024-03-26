BUILD    ?= build
SRC      := src

CFLAGS   := -Wall -I$(SRC) -lm -g

sources  := $(wildcard $(SRC)/*.c)

# Pega os nomes dos arquivos remove a extensão e diretório
objects  := $(notdir $(basename $(sources)))
# Concatena .o no final
objects  := $(addsuffix .o, $(objects))
# Contatena $BUILD/ no começo
objects  := $(addprefix $(BUILD)/, $(objects))

tp1: $(BUILD) $(objects)
	$(CC) $(objects) -o $@

$(objects): $(BUILD)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
	rm tp1

.PHONY: clean

