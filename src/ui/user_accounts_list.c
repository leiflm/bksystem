#include <Elementary.h>
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Elm_Utils.h"

void products_page_reset
{
   products_list_reset(ui.products.list);
}

static void
_on_user_account_select(void *data, Evas_Object *obj, void *event_info)
{
   const Bks_Model_User_Account *acc = (const Bks_Model_User_Account*)data;
   Bks_Ui *ui = (Bks_Ui*)event_info;

   if (!ui || !data) return;

   printf("Account %s, %s was selected\n", acc->lastname, acc->firstname);

   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_FALSE);
}

static void
_on_user_account_double_click(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Ui *ui = (Bks_Ui*)event_info;

   EINA_SAFETY_ON_NULL_RETURN(ui);

   if (eina_list_count(elm_list_selected_items_get(ui.user_accounts.enp.content)) > 1)
         return;
   bks_controller_sale_finish(ctrl);
   products_page_reset(ui);
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

void
user_accounts_list_fill(const Bks_Model *model)
{
   Eina_List *iter;
   Bks_Model_User_Account *acc;
   char buf[256];

   if (!ui || !model) return;

   if (ui.user_accounts.list)
     elm_list_clear(ui.user_accounts.list);
   EINA_LIST_FOREACH(mdl.user_accounts, iter, acc)
     {
        /*
         * Disable icons for now
         ic = elm_icon_add(ui.user_accounts.list);
         snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
         elm_icon_file_set(ic, buf, NULL);
         elm_icon_scale_set(ic, 1, 1);
         li = elm_list_item_append(ui.user_accounts.list, name, ic, NULL, on_user_account_select, name);
         */
        snprintf(buf, sizeof(buf), "%s, %s", acc->lastname, acc->firstname);
        elm_list_item_append(ui.user_accounts.list, buf, NULL, NULL, _on_user_account_select, acc);
     }

}

Evas_Object *_user_accounts_page_list_add
{
   Evas_Object *list;

   if (!ui || !ui.win) return NULL;

   list = elm_list_add(ui.win);
   elm_list_multi_select_set(list, EINA_TRUE);

   // Add callback for doubleclick action
   evas_object_smart_callback_add(ui.user_accounts.enp.content, "clicked,double", _on_user_account_double_click, ui);

   elm_list_go(list);

   return list;
}
