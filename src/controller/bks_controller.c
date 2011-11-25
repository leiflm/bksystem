#include "Bks_Ui.h"
#include <Eina.h>

/* REPLACE ME BY A SQL TRANSACTION AND UI INTERFACE CALL! */
void
print_sale(void)
{
   Eina_List *iter;
   const Bks_User_Account *acc;
   const Bks_Product *product;

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
