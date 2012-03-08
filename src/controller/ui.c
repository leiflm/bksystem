#include <Eina.h>
#include <Ecore.h>
#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Controller_Private.h"

/*
 * @brief sends sale to model
 */
void _sale_finish_cb(void *data UNUSED, Ecore_Thread *th UNUSED)
{
   const Bks_Model_User_Account *acc = NULL;
   Bks_Model_Sale *sale = NULL;

   EINA_SAFETY_ON_NULL_GOTO(ctrl.ui_selection.product, _incomplete_sale);
   EINA_SAFETY_ON_NULL_GOTO(ctrl.ui_selection.user_accounts, _incomplete_sale);

   EINA_LIST_FREE(ctrl.ui_selection.user_accounts, acc)
     {
        sale = bks_model_sale_new(acc->uid, ctrl.ui_selection.product->EAN, ctrl.ui_selection.product->price);
        bks_model_commit_sale(sale);
     }

   ctrl.ui_selection.user_accounts = NULL;
   ctrl.ui_selection.product = NULL;

   return;

_incomplete_sale:
   printf("Die UI sagt, dass ein Produkt gekauft wurde, allerdings fehlt das Produkt(%p) oder die Kaeufer(%p).\n", ctrl.ui_selection.product, ctrl.ui_selection.user_accounts);
}

// API for UI callbacks
void bks_controller_ui_sale_finish_cb(void)
{
   ctrl.ui_selection.product = bks_ui_product_selected_get();
   ctrl.ui_selection.user_accounts = bks_ui_user_accounts_selected_get();
   //CREATE ECORE_THREAD that calls ui cb when finished
   ecore_thread_run(_sale_finish_cb, NULL, NULL, NULL);
}
