TOP_DIR = ..
BUILD_DIR = $(TOP_DIR)/build
TOP_SRC_DIR = $(TOP_DIR)/src
TOP_HEADERS_DIR = $(TOP_SRC_DIR)/include

COMMON_SRC_DIR = $(TOP_SRC_DIR)/common
COMMON_SRC = $(COMMON_SRC_DIR)/bks_job.c $(COMMON_SRC_DIR)/bks_status.c

CTRL_SRC_DIR = $(TOP_SRC_DIR)/controller
CTRL_HEADERS_DIR = $(CTRL_SRC_DIR)/include
CTRL_SRC = $(CTRL_SRC_DIR)/bks_controller.c
CTRL_CFLAGS = -I$(CTRL_HEADERS_DIR)

MODEL_SRC_DIR = $(TOP_SRC_DIR)/model
MODEL_HEADERS_DIR = $(MODEL_SRC_DIR)/include
MODEL_HEADERS = $(MODEL_HEADERS_DIR)/Bks_Model_Path.h $(MODEL_HEADERS_DIR)/Bks_Model_Sql.h
MODEL_SRC = $(MODEL_SRC_DIR)/bks_model.c $(MODEL_SRC_DIR)/bks_model_path.c $(MODEL_SRC_DIR)/bks_model_user_account.c $(MODEL_SRC_DIR)/bks_model_product.c $(MODEL_SRC_DIR)/bks_model_sale.c $(MODEL_SRC_DIR)/bks_model_sql.c
MODEL_CFLAGS = -I$(MODEL_HEADERS_DIR)

UI_SRC_DIR = $(TOP_SRC_DIR)/ui
UI_HEADERS_DIR = $(UI_SRC_DIR)/include
UI_HEADERS = $(UI_HEADERS_DIR)/Bks_Notification.h $(UI_HEADERS_DIR)/Bks_Ui_Private.h $(UI_HEADERS_DIR)/Elm_Utils.h
UI_COMMON_SRC = $(UI_SRC_DIR)/elm_utils.c $(UI_SRC_DIR)/win.c $(UI_SRC_DIR)/bks_ui.c $(UI_SRC_DIR)/controller.c $(UI_SRC_DIR)/fileselector.c $(UI_SRC_DIR)/notification.c
UI_PRODUCTS_SRC = $(UI_SRC_DIR)/products_grid.c $(UI_SRC_DIR)/products_list.c $(UI_SRC_DIR)/products_page.c
UI_USER_ACCOUNTS_SRC = $(UI_SRC_DIR)/user_accounts_list.c $(UI_SRC_DIR)/user_accounts_page.c
UI_SRC = $(UI_COMMON_SRC) $(UI_PRODUCTS_SRC) $(UI_USER_ACCOUNTS_SRC)
UI_CFLAGS = -I$(UI_HEADERS_DIR)

MAIN_SRC_DIR = $(TOP_SRC_DIR)
MAIN_HEADERS = $(TOP_HEADERS_DIR)/Bks_Controller.h $(TOP_HEADERS_DIR)/Bks_Job.h $(TOP_HEADERS_DIR)/Bks_Model.h $(TOP_HEADERS_DIR)/Bks_Model_Product.h $(TOP_HEADERS_DIR)/Bks_Model_Sale.h $(TOP_HEADERS_DIR)/Bks_Model_User_Account.h $(TOP_HEADERS_DIR)/Bks_Status.h $(TOP_HEADERS_DIR)/Bks_System.h $(TOP_HEADERS_DIR)/Bks_Types.h $(TOP_HEADERS_DIR)/Bks_Ui.h

CFLAGS = -I$(TOP_HEADERS_DIR) $(CTRL_CFLAGS) $(MODEL_CFLAGS) $(UI_CFLAGS)
DEBUG_CFLAGS = -ggdb -W -Wall -Wshadow
SYSTEM_CFLAGS = `pkg-config --cflags elementary,sqlite3`
SYSTEM_LDFLAGS = `pkg-config --libs elementary,sqlite3`

MAIN_SRC = $(MAIN_SRC_DIR)/main.c

SRC = $(MAIN_SRC) $(COMMON_SRC) $(CTRL_SRC) $(MODEL_SRC) $(UI_SRC)
HEADERS = $(MAIN_HEADERS) $(MODEL_HEADERS) $(UI_HEADERS)

MAIN_OBJ = $(BUILD_DIR)/bksystem

DOXY_DIR = $(TOP_DIR)/doc/doxygen
DOXY_CONF = $(DOXY_DIR)/doxyconf
DOXY_TEMPL = $(DOXY_DIR)/headerFile $(DOXY_DIR)/footerFile

all: $(MAIN_OBJ)

doc: $(SRC) $(HEADERS) $(DOXY_CONF) $(DOXY_TEMPL)
	doxygen $(DOXY_CONF)

$(MAIN_OBJ): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $(SYSTEM_CFLAGS) $(SYSTEM_LDFLAGS) $(SRC) -o $(MAIN_OBJ)
