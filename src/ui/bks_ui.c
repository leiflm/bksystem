#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Types.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"

Bks_Ui bks_ui_init(void)
{
   // initialize Elementary
   elm_init(0, NULL);

   // new window - do the usual and give it a name and title
   ui.win = bks_ui_win_new();

   // add naviframe
   ui.naviframe = elm_naviframe_add(ui.win);
   evas_object_show(ui.naviframe);
   // add object as a resize object for the window (controls window minimum
   // size as well as gets resized if window is resized)
   elm_win_resize_object_add(ui.win, ui.naviframe);

   //create and fill products
   ui.products.enp.content = products_page_add(ui);
   ui.products.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Produkte", NULL, NULL, ui.products.enp.content, NULL);

   //create, fill and add user_accounts ui elements
   ui.user_accounts.enp.content = user_accounts_page_add(ui);
   ui.user_accounts.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Konten", ui.user_accounts.enp.prev_btn, ui.user_accounts.enp.next_btn, ui.user_accounts.enp.content, NULL);

   elm_naviframe_item_promote(ui.products.enp.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui.naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(ui.win, 640, 480);
   evas_object_show(ui.win);

   return ui;
}

int bks_ui_run(void)
{
   /*
    * Here goes some code that indicates that we are waiting for data to be
    * available
    */

   // get data for the ui CAUTION: This call blocks until data is available
   bks_model_data_get();

   bks_ui_fill();

   // run the mainloop and process events and callbacks
   elm_run();

   return 0;
}

void bks_ui_shutdown(void)
{
   evas_object_del(ui.products.enp.content);
   ui.products.enp.content = NULL;
   evas_object_del(ui.user_accounts.enp.content);
   ui.user_accounts.enp.content = NULL;
   evas_object_del(ui.naviframe);
   ui.naviframe = NULL;

   elm_shutdown();
}

/**
 * @param ui The Bks_Ui the selected user accounts are retrieved from
 * @return List of Bks_Model_User_Account elements
 */
Eina_List *bks_ui_user_accounts_selected_get(void)
{
   Eina_List *selected_accounts, *iter, *list = NULL;
   Elm_List_Item *eli;
   const Bks_Model_User_Account *acc;

   if (!ui) return NULL;

   if (!(selected_accounts = (Eina_List*)elm_list_selected_items_get(ui.user_accounts.list)))
       return NULL;

   // print the names of all selected accounts
   EINA_LIST_FOREACH(selected_accounts, iter, eli)
     {
        if (elm_list_item_selected_get(eli))
          {
             acc = (Bks_Model_User_Account*)elm_list_item_data_get(eli);
             printf("Selected User Account: %s, %s\n", acc->lastname, acc->firstname);
             list = eina_list_append(list, acc);
          }
     }

   return list;
}

const Bks_Model_Product *bks_ui_product_selected_get(void)
{
   Elm_List_Item *selected_product;
   const Bks_Model_Product *product;

   if (!(selected_product = elm_list_selected_item_get(ui.products.list)))
     return NULL;

   product = (Bks_Model_Product*)elm_list_item_data_get(selected_product);

   return product;
}

void bks_ui_fill(void)
{
   products_page_fill(ui, model);
   user_accounts_page_fill(ui, model);
}
