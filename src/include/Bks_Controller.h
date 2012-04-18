#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"

struct _Bks_Controller {
     struct {
          const Bks_Model_Product *product;
          Eina_List *user_accounts;
     } ui_selection;
};

// Adminstrative calls
void bks_controller_init(void);
void bks_controller_shutdown(void);
void bks_controller_run(void);

// Business logic
void bks_controller_ui_db_path_set(Eina_Stringshare* db_path);
void bks_controller_ui_product_selected_cb(void);
void bks_controller_ui_users_selected_cb(void);
void bks_controller_ui_sale_finish_cb(void);
void bks_controller_bill_create_cb(const Bks_Model_User_Account *userAccount);

// Model callbacks
void bks_controller_model_db_path_get(void);
void bks_controller_model_reload_cb(void);
void bks_controller_model_user_accounts_get_cb(void);
void bks_controller_model_user_accounts_get_finished_cb(Eina_List *user_accounts, Bks_Error error);
void bks_controller_model_products_alpha_get_cb(void);
void bks_controller_model_products_alpha_get_finished_cb(Eina_List *products_alpha, Bks_Error error);
void bks_controller_model_products_favs_get_cb(void);
void bks_controller_model_products_favs_get_finished_cb(Eina_List *products_favs, Bks_Error error);
void bks_controller_model_commit_sale_finished_cb(Bks_Model_Sale *sale);
void bks_controller_model_create_bill_table_cb(void);

#endif
