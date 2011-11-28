#include "Bks_Controller_Private.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"
#include "Bks_Model.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_User_Account.h"
#include <Eina.h>

extern Bks_Controller *controller;

Bks_Controller *bks_controller_new(Bks_Model *model)
{
   if (!model) return NULL;
   if (controller) return controller;

   controller = calloc(sizeof(*controller));

   bks_controller_model_set(model);

}

/* REPLACE ME BY A SQL TRANSACTION AND UI INTERFACE CALL! */
   void
print_sale(void)
{
   Eina_List *iter;
   const Bks_Model_User_Account *acc;
   const Bks_Model_Product *product;

   product = bks_ui_product_selected_get();

   printf("%s was bought by:\n", product->name);
   // print the names of all selected accounts
   EINA_LIST_FOREACH(bks_ui_user_accounts_selected_get, iter, acc)
     {
        printf("\t%s, %s\n", acc->lastname, acc->firstname);
     }
}
/* END REPLACE */

void bks_controller_sale_finish(const Bks_Controller *controller)
{
   printSale(controller);
}

void bks_controller_run(void)
{
   if (!controller) return;

   bks_ui_run();
}
