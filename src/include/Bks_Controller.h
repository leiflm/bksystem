#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"
#include "Bks_Error.h"

struct _Bks_Controller {
    Bks_Model *model;
    Bks_Ui *ui;
};

// Adminstrative calls
void bks_controller_init(void);
void bks_controller_shutdown(void);

// Business logic
void bks_controller_ui_product_selected_cb(void);
void bks_controller_ui_users_selected_cb(void);
void bks_controller_bill_create_cb(const Bks_Model_User_Account *userAccount);

// Model callbacks
void bks_controller_model_reloaded_cb(void);
void bks_controller_model transaction_finished_cb(void);
void bks_controller_model_create_bill_table_cb(void);

#endif
