TOP_DIR = .
DATA_DIR = $(TOP_DIR)/data
BUILD_DIR = $(TOP_DIR)/build
SRC_DIR = $(TOP_DIR)/src
BIN = $(BUILD_DIR)/bksystem
DB = $(BUILD_DIR)/bksystem.sqlite

.PHONY: $(BIN)

all: $(BIN) $(DB)

$(BIN):
	make -C $(SRC_DIR);
	cd ..;

$(DB):
	cp $(DATA_DIR)/bksystem.sqlite $@

clean:
	rm -f $(BIN) $(DB)
