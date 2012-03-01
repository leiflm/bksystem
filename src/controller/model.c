#include <Ecore.h>
#include "Bks_Ui.h"
#include "Bks_Controller_Private.h"

void _bks_controller_model_products_reload_cb(void *data, Ecore_Thread *th)
{
   //lock ui from further user input
   bks_ui_products_update_set(EINA_TRUE);
   //clear ui
   bks_ui_products_clear();
}

void _bks_controller_model_user_accounts_reload_cb(void *data, Ecore_Thread *th)
{
   //lock ui from further user input
   bks_ui_user_accounts_update_set(EINA_TRUE);
   //clear ui
   bks_ui_user_accounts_clear();
}
