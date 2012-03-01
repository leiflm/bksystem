#include <Eina.h>
#include <Ecore.h>
#include "Bks_Types.h"
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
void _sale_finish_cb(void *data, Ecore_Thread *th)
{
   const Bks_Model_Product *prod = bks_ui_product_selected_get();
   Eina_List *accs = bks_ui_user_accounts_selected_get();
   const Bks_Model_User_Account *acc = NULL;
   Bks_Model_Sale *sale = NULL;

   EINA_SAFETY_ON_NULL_GOTO(prod, _incomplete_sale);
   EINA_SAFETY_ON_NULL_GOTO(accs, _incomplete_sale);

   EINA_LIST_FREE(accs, acc)
     {
        sale = bks_model_sale_new(acc->uid, prod->EAN, prod->price);
        bks_model_commit_sale(sale);
     }

   return;

_incomplete_sale:
   printf("Die UI sagt, dass ein Produkt gekauft wurde, allerdings fehlen Produkt(%p) oder Kaeufer(%p).\n", prod, accs);
}

// API for UI callbacks
void bks_controller_ui_sale_finish_cb(void)
{
   //CREATE ECORE_THREAD that calls ui cb when finished
   ecore_thread_run(_sale_finish_cb, NULL, NULL, NULL);
}
