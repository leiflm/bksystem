#include <Eina.h>
#include <Ecore.h>
#include "Bks_Model_Sale.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

// API callbacks
void bks_ui_controller_sale_finished(Bks_Model_Sale *sale)
{
   Eina_List *l, *accs = (Eina_List*)sale->user_accounts;
   Bks_Model_User_Account *acc = NULL;

   EINA_SAFETY_ON_NULL_RETURN(sale);
   bks_ui_sale_notification_set(sale);
   EINA_LIST_FOREACH(accs, l, acc)
      printf("Produktkauf %llu durch %s, %s wurde in Rechnung gestellt.\n", sale->EAN, acc->lastname, acc->firstname);

   bks_model_sale_free(sale);
}
