#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Elm_Utils.h"
#include <Elementary.h>

extern Bks_Ui *ui;

static products_page_reset(void)
{
   _products_page_list_reset();
}

static void
_on_user_accounts_select(void *data, Evas_Object *obj, void *event_info)
{
   const Bks_User_Account *acc = (const Bks_User_Account*)data;

   if (!ui || !data) return;

   printf("Account %s, %s was selected\n", acc->lastname, acc->firstname);

   elm_widget_disabled_set(ui->user_accounts.enp.next_btn, EINA_FALSE);
}

static void
_on_user_accounts_double_click(void *data, Evas_Object *obj, void *event_info)
{
   if (!ui || !data) return;

   if (eina_list_count(elm_list_selected_items_get(ui->user_accounts.enp.content)) > 1)
         return;
   bks_controller_sale_finish(ui->controller);
   products_page_reset();
   elm_naviframe_item_promote(ui->productslist.enp.eoi);
}

static void
_user_accounts_fill(void)
{
   Evas_Object *ic;
   Elm_List_Item *li;
   Eina_List *iter;
   Bks_User_Account *acc;
   char buf[256];

   if (!ui) return;

   EINA_LIST_FOREACH(ui->model->user_accounts, iter, acc)
     {
        /*
         * Disable icons for now
         ic = elm_icon_add(ui->user_accounts.list);
         snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
         elm_icon_file_set(ic, buf, NULL);
         elm_icon_scale_set(ic, 1, 1);
         li = elm_list_item_append(ui->user_accounts.list, name, ic, NULL, on_user_account_select, name);
         */
        snprintf(buf, sizeof(buf), "%s, %s", acc->lastname, acc->firstname);
        li = elm_list_item_append(ui->user_accounts.list, buf, NULL, NULL, _on_user_account_select, acc);
     }

}

Evas_Object *_user_accounts_page_list_add(void)
{
   Evas_Object *list;

   if (!ui || !ui->win) return NULL;

   list = elm_list_add(ui->win);
   elm_list_multi_select_set(list, EINA_TRUE);
   _user_accounts_fill();

   // Add callback for doubleclick action
   evas_object_smart_callback_add(ui->user_accounts.enp.content, "clicked,double", _on_user_account_double_click, NULL);

   elm_list_go(list);

   return list;
}
