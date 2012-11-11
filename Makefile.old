TOP_DIR = .
DATA_DIR = $(TOP_DIR)/data
BUILD_DIR = $(TOP_DIR)/build
SRC_DIR = $(TOP_DIR)/src
DOC_DIR = $(TOP_DIR)/doc
DOC = $(DOC_DIR)/html/index.html
BIN = $(BUILD_DIR)/bksystem
DB = $(BUILD_DIR)/bksystem.sqlite
IMAGES = $(BUILD_DIR)/images

.PHONY: $(BIN) $(DOC)

all: $(BUILD_DIR) $(BIN) $(DB) $(IMAGES) $(DOC)

bin: $(BIN)

doc: $(DOC)

$(BUILD_DIR):
	mkdir $@

$(BIN):
	make -C $(SRC_DIR);

$(DB):
	cp $(DATA_DIR)/bksystem.sqlite $@

$(IMAGES):
	cp -R $(DATA_DIR)/images $@

$(DOC):
	make doc -C $(SRC_DIR)

clean:
	rm -f $(BIN) $(DB) $(DOC)
