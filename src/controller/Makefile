TOP_DIR = ../../
BUILD_DIR = $(TOP_DIR)/build
TOP_SRC_DIR = $(TOP_DIR)/src
TOP_HEADERS_DIR = $(TOP_SRC_DIR)/include
CTRL_SRC_DIR = ./
CTRL_HEADERS_DIR = $(CTRL_SRC_DIR)/include

CFLAGS = -ggdb -Wall -shared -I$(TOP_HEADERS_DIR) -I$(CTRL_HEADERS_DIR)
CTRL_CFLAGS = `pkg-config --cflags eina,ecore,elementary,sqlite3`
CTRL_LDFLAGS = `pkg-config --libs eina,ecore,elementary,sqlite3`

CTRL_SRC = bks_controller.c model.c

#CTRL_OBJ = bks_controller.o model.o

all: bks_controller.o

bks_controller.o: $(CTRL_SRC)
	$(CC) $(CFLAGS) $(CTRL_CFLAGS) $(CTRL_SRC) $(CTRL_LDFLAGS) -o $(BUILD_DIR)/bks_controller.o

#%.o: %.c
#	$(CC) $(CFLAGS) $(CTRL_CFLAGS) $(CTRL_LDFLAGS) -c $< -o $(BUILD_DIR)/$@
