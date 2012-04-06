TOP_DIR = .
DATA_DIR = $(TOP_DIR)/data
BUILD_DIR = $(TOP_DIR)/build
SRC_DIR = $(TOP_DIR)/src
BIN = $(BUILD_DIR)/bksystem
DB = $(BUILD_DIR)/bksystem.sqlite
IMAGES = $(BUILD_DIR)/images

.PHONY: $(BIN)

all: $(BUILD_DIR) $(BIN) $(DB) $(IMAGES)

$(BUILD_DIR):
	mkdir $@

$(BIN):
	make -C $(SRC_DIR);
	cd ..;

$(DB):
	cp $(DATA_DIR)/bksystem.sqlite $@

$(IMAGES):
	cp -R $(DATA_DIR)/images $@

clean:
	rm -f $(BIN) $(DB)
