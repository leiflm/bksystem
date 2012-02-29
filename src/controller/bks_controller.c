#include <Eina.h>
#include <Ecore.h>
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Controller.h"

void bks_controller_init(void)
{
}

void bks_controller_shutdown(void)
{
}

/* REPLACE ME BY A SQL TRANSACTION AND UI INTERFACE CALL! */
   void
print_sale(void)
{
   Eina_List *accounts;
   const Bks_Model_User_Account *acc;
   const Bks_Model_Product *product;

   product = bks_ui_product_selected_get();
   accounts = bks_ui_user_accounts_selected_get();
   printf("%s was bought by:\n", product->name);
   // print the names of all selected accounts
   EINA_LIST_FREE(accounts, acc)
     {
        printf("\t%s, %s\n", acc->lastname, acc->firstname);
     }
}
/* END REPLACE */

void bks_controller_ui_sale_finish_cb(void)
{
   //CREATE ECORE_THREAD that calls ui cb when finished
   print_sale();
}

void bks_controller_run(void)
{
   ecore_main_loop_begin();
}
