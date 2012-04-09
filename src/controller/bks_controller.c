#include <Eina.h>
#include <Ecore.h>
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Controller.h"
#include "Bks_Controller_Private.h"

void bks_controller_init(void)
{
}

void bks_controller_shutdown(void)
{
}

void bks_controller_run(void)
{
   bks_model_products_get(0);
   bks_model_products_get(5);
   bks_model_user_accounts_get(0);
   ecore_main_loop_begin();
}
