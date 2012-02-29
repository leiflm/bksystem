TOP_DIR = .
BUILD_DIR = $(TOP_DIR)/build
SRC_DIR = $(TOP_DIR)/src
BIN = $(BUILD_DIR)/bks_system

all: $(BIN)

$(BIN):
	make -C $(SRC_DIR);
	cd ..;
