#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"

struct _Bks_Controller {
    Eina_Lock db_lock;
    Eina_Bool is_primary_instance;
};

// Adminstrative calls
void bks_controller_init(void);
void bks_controller_shutdown(void);
void bks_controller_run(void);

// Business logic
void bks_controller_ui_db_path_retrieved(Eina_Stringshare *path);
void bks_controller_ui_product_selected(void);
void bks_controller_ui_users_selected(void);
void bks_controller_ui_sale_finish(void);
void bks_controller_bill_create_cb(const Bks_Model_User_Account *userAccount);

void bks_controller_products_alpha_get(void);
void bks_controller_products_favs_get(unsigned int limit);
void bks_controller_user_accounts_alpha_get(void);
void bks_controller_user_accounts_favs_get(unsigned int limit);

void bks_controller_ui_start_even_if_lock_exists(void);
#endif
