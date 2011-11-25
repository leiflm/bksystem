#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Bks_Product.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"

extern Bks_Ui *ui;

extern const Evas_Object *bks_ui_win_create(void);

Bks_Ui *bks_ui_new(const Bks_Controller controller, const Bks_Model *model)
{
   Bks_Ui *bks_ui = NULL;

   if (!controller || !model) return NULL;

   bks_ui = calloc(sizeof(*bks_ui));

   bks_ui->controller = controller;
   bks_ui->model = model;
   bks_ui->run = run;

   bks_controller_bks_ui_set(bks_ui->controller, bks_ui);

   ui = bks_ui;

   return ui;
}

void bks_ui_run(void)
{
   /*
    * Here goes some code that indicates that we are waiting for data to be
    * available
    */

   // get data for the ui CAUTION: This call blocks until data is available
   bks_model_data_get(model);

   // new window - do the usual and give it a name and title
   ui->win = bks_ui_win_new();

   // add naviframe
   ui->naviframe = elm_naviframe_add(ui->win);
   evas_object_show(ui->naviframe);
   // add object as a resize object for the window (controls window minimum
   // size as well as gets resized if window is resized)
   elm_win_resize_object_add(ui->win, ui->naviframe);

   //create and fill products
   products_page_add();

   //create, fill and add user_accounts ui elements
   user_accounts_page_add();

   elm_naviframe_item_promote(ui->products.enp.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui->naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(ui->win, 640, 480);
   evas_object_show(ui->win);

   // run the mainloop and process events and callbacks
   elm_run();

   evas_object_del(ui->products.enp.content);
   evas_object_del(ui->user_accounts.enp.content);
   evas_object_del(ui->naviframe);

   return 0;
}

void bks_ui_shutdown(void)
{
}

/**
 * @param ui The Bks_Ui the selected user accounts are retrieved from
 * @return List of Bks_User_Account elements
 */
Eina_List *bks_ui_user_accounts_selected_get(void)
{
   Eina_List *selected_accounts, *iter, *list = NULL;
   Elm_List_Item *eli;

   if (!ui) return NULL;

   if (!(selected_accounts = (Eina_List*)elm_list_selected_items_get(ui.accountslist.enp.content)))
       return NULL;

   // print the names of all selected accounts
   EINA_LIST_FOREACH(selected_accounts, iter, eli)
     {
        if (elm_list_item_selected_get(eli))
          {
             acc = (char*)elm_list_item_data_get(eli);
             printf("Selected User Account: %s, %s\n", acc->lastname, acc->firstname);
             list = eina_list_append(list, acc);
          }
     }

   return list;
}

const Bks_Product *bks_ui_product_selected_get(void)
{
   Elm_List_Item *selected_product;
   const Bks_Product *product;

   if (!(selected_product = elm_list_selected_item_get(ui.productslist.list)))
     return NULL;

   product = (Bks_Product*)elm_list_item_data_get(selected_product);

   return product;
}
