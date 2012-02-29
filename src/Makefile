TOP_DIR = ..
BUILD_DIR = $(TOP_DIR)/build
TOP_SRC_DIR = $(TOP_DIR)/src
TOP_HEADERS_DIR = $(TOP_SRC_DIR)/include
UI_SRC_DIR = $(TOP_SRC_DIR)/ui
CTRL_SRC_DIR = $(TOP_SRC_DIR)/controller
MODEL_SRC_DIR = $(TOP_SRC_DIR)/model
MAIN_SRC_DIR = $(TOP_SRC_DIR)

CFLAGS = -ggdb -Wall -I$(TOP_HEADERS_DIR)
SYSTEM_CFLAGS = `pkg-config --cflags elementary,sqlite3`
SYSTEM_LDFLAGS = `pkg-config --libs elementary,sqlite3`

MODEL_OBJ = $(BUILD_DIR)/bks_model.o
CTRL_OBJ = $(BUILD_DIR)/bks_controller.o
UI_OBJ = $(BUILD_DIR)/bks_ui.o
OBJS = $(MODEL_OBJ) $(CTRL_OBJ) $(UI_OBJ)
MAIN_OBJ = $(BUILD_DIR)/bks_system

MAIN_SRC = $(MAIN_SRC_DIR)/main.c

all: $(MAIN_OBJ)

$(MAIN_OBJ): $(OBJS)
	$(CC) $(CFLAGS) $(SYSTEM_CFLAGS) $(SYSTEM_LDFLAGS) $(OBJS) $(MAIN_SRC) -o $(MAIN_OBJ)

$(MODEL_OBJ):
	make -C $(MODEL_SRC_DIR);
	cd ..;

$(CTRL_OBJ):
	make -C $(CTRL_SRC_DIR);
	cd ..;

$(UI_OBJ):
	make -C $(UI_SRC_DIR);
	cd ..;
