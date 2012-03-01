#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"

struct _Bks_Controller {
    Bks_Model *model;
    Bks_Ui *ui;
};

// Adminstrative calls
void bks_controller_init(void);
void bks_controller_shutdown(void);
void bks_controller_run(void);

// Business logic
void bks_controller_ui_product_selected_cb(void);
void bks_controller_ui_users_selected_cb(void);
void bks_controller_ui_sale_finish_cb(void);
void bks_controller_bill_create_cb(const Bks_Model_User_Account *userAccount);

// Model callbacks
void bks_controller_model_reload_cb(void);
void bks_controller_model_user_accounts_reload_cb(void);
void bks_controller_model_products_reload_cb(void);
void bks_controller_model_reload_finished_cb(void);
void bks_controller_model_user_accounts_reload_finished_cb(void);
void bks_controller_model_products_reload_finished_cb(void);
void bks_controller_model_sale_finished_cb(Bks_Model_Sale *sale);
void bks_controller_model_create_bill_table_cb(void);

#endif
