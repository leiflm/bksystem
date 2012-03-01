TOP_DIR = ../../
BUILD_DIR = $(TOP_DIR)/build
TOP_SRC_DIR = $(TOP_DIR)/src
TOP_HEADERS_DIR = $(TOP_SRC_DIR)/include
MODEL_SRC_DIR = $(TOP_SRC_DIR)/model
MODEL_HEADERS_DIR = $(MODEL_SRC_DIR)/include

CFLAGS = -ggdb -Wall -shared -I$(TOP_HEADERS_DIR) -I$(MODEL_HEADERS_DIR)
MODEL_CFLAGS = `pkg-config --cflags eina,ecore,elementary,sqlite3`
MODEL_LDFLAGS = `pkg-config --libs eina,ecore,elementary,sqlite3`

MODEL_SRC = bks_model.c bks_model_user_account.c bks_model_product.c bks_model_sale.c bks_model_threading.c bks_model_sql.c

all: bks_model.o

bks_model.o: $(MODEL_SRC)
	$(CC) $(CFLAGS) $(MODEL_CFLAGS) $(MODEL_SRC) $(MODEL_LDFLAGS) -o $(BUILD_DIR)/bks_model.o

#%.o: %.c
#	$(CC) $(CFLAGS) $(MODEL_CFLAGS) $(MODEL_LDFLAGS) -c $<  -o $(BUILD_DIR)/$@
