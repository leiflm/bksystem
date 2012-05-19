#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"

// Adminstrative calls
void bks_controller_init(void);
void bks_controller_shutdown(void);
void bks_controller_run(void);

// Business logic
void bks_controller_ui_db_path_retrieved(Bks_Job *job);
void bks_controller_ui_product_selected(void);
void bks_controller_ui_users_selected(void);
void bks_controller_ui_sale_finish(void);
void bks_controller_bill_create_cb(const Bks_Model_User_Account *userAccount);

#endif
