TOP_DIR = ../../
BUILD_DIR = $(TOP_DIR)/build
TOP_SRC_DIR = $(TOP_DIR)/src
TOP_HEADERS_DIR = $(TOP_SRC_DIR)/include
UI_SRC_DIR = ./
UI_HEADERS_DIR = $(UI_SRC_DIR)/include

CFLAGS = -ggdb -Wall -I$(TOP_HEADERS_DIR) -I$(UI_HEADERS_DIR) -shared
UI_CFLAGS = `pkg-config --cflags elementary,sqlite3`
UI_LDFLAGS = `pkg-config --libs elementary,sqlite3`

COMMON_SRC = elm_utils.c win.c bks_ui.c
PRODUCTS_SRC = products_grid.c products_list.c products_page.c
USER_ACCOUNTS_SRC = user_accounts_list.c user_accounts_page.c
UI_SRC = $(COMMON_SRC) $(PRODUCTS_SRC) $(USER_ACCOUNTS_SRC)

#UI_OBJS = $(COMMON_OBJS) $(PRODUCTS_OBJS) $(USER_ACCOUNTS_OBJS)

all: bks_ui.o

bks_ui.o: $(UI_SRC)
	$(CC) $(CFLAGS) $(UI_CFLAGS) $(UI_LDFLAGS) $(UI_SRC) -o $(BUILD_DIR)/bks_ui.o

#%.o: %.c
#	$(CC) $(CFLAGS) $(UI_CFLAGS) $(UI_LDFLAGS) -c $< -o $(BUILD_DIR)/$@
