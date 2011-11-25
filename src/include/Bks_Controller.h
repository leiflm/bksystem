#ifndef __BKS_CONTROLLER_H__
#define __BKS_CONTROLLER_H__

#include "Bks_User_Account.h"
#include "Bks_Error.h"

// Adminstrative calls
Bks_Controller *bks_controller_add(const Bks_Model *model);
void bks_controller_model_set(const Bks_Controller *controller, const Bks_Ui *ui);
void bks_controller_ui_set(const Bks_Controller *controller, const Bks_Ui *ui);
void bks_controller_shutdown(const Bks_Controller *controller);

// Business logic
void bks_controller_sale_finish(const Bks_Controller *controller);
void bks_controller_bill_create(const Bks_Controller *controller, const Bks_User_Account *userAccount);

#endif
