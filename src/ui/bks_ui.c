#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Model_Product.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void bks_ui_init(void)
{
   // initialize Elementary
   elm_init(0, NULL);

   // new window - do the usual and give it a name and title
   ui.win = bks_ui_win_add();

   // add naviframe
   ui.naviframe = elm_naviframe_add(ui.win);
   evas_object_show(ui.naviframe);
   // add object as a resize object for the window (controls window minimum
   // size as well as gets resized if window is resized)
   elm_win_resize_object_add(ui.win, ui.naviframe);

   //create and fill products
   ui.products.enp.content = products_page_add();
   ui.products.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Produkte", NULL, NULL, ui.products.enp.content, NULL);

   //create, fill and add user_accounts ui elements
   ui.user_accounts.enp.content = user_accounts_page_add();
   ui.user_accounts.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Konten", ui.user_accounts.enp.prev_btn, ui.user_accounts.enp.next_btn, ui.user_accounts.enp.content, NULL);

   elm_naviframe_item_promote(ui.products.enp.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui.naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(ui.win, 640, 480);
   evas_object_show(ui.win);
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
   const Eina_List *selected_accounts;
   Eina_List *iter, *list = NULL;
   Elm_Object_Item *eoi;
   const Bks_Model_User_Account *acc;

   if (!(selected_accounts = elm_list_selected_items_get(ui.user_accounts.list)))
       return NULL;

   // print the names of all selected accounts
   EINA_LIST_FOREACH((Eina_List*)selected_accounts, iter, eoi)
     {
        if (elm_list_item_selected_get(eoi))
          {
             acc = (Bks_Model_User_Account*)elm_object_item_data_get(eoi);
             printf("Selected User Account: %s, %s\n", acc->lastname, acc->firstname);
             list = eina_list_append(list, acc);
          }
     }

   return list;
}

const Bks_Model_Product *bks_ui_product_selected_get(void)
{
   Elm_Object_Item *selected_product;
   const Bks_Model_Product *product;

   if (!(selected_product = elm_list_selected_item_get(ui.products.list)))
     return NULL;

   product = (Bks_Model_Product*)elm_object_item_data_get(selected_product);

   return product;
}

void bks_ui_products_page_update(const Eina_List *products)
{
   //CREATE THREAD
   products_page_clear();
   products_page_fill(products);
}

void bks_ui_user_accounts_page_update(const Eina_List *user_accounts)
{
   //CREATE THREAD
   user_accounts_page_clear();
   user_accounts_page_fill(user_accounts);
}

void bks_ui_model_sale_finished_cb(Bks_Model_Sale *sale)
{
   printf("Sale of %s to %s, %s finished with state: %llui.\n", sale->productname, sale->lastname, sale->firstname, sale->status);
}
