#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

static void
_on_user_account_select(void *data, Evas_Object *obj, void *event_info)
{
   Elm_Object_Item *li = (Elm_Object_Item*)event_info;
   const Bks_Model_User_Account *acc = NULL;

   EINA_SAFETY_ON_NULL_RETURN(li);
   acc = (const Bks_Model_User_Account*)elm_object_item_data_get(li);
   EINA_SAFETY_ON_NULL_RETURN(acc);

   printf("Account %s, %s was selected\n", acc->lastname, acc->firstname);

   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_FALSE);
}

static void
_on_user_account_double_click(void *data, Evas_Object *obj, void *event_info)
{
   //Only react if only a single account is selected
   if (eina_list_count(elm_list_selected_items_get(ui.user_accounts.enp.content)) > 1)
     return;
   bks_controller_ui_sale_finish_cb();
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

void
user_accounts_list_set(Eina_List *user_accounts)
{
   Bks_Model_User_Account *acc;
   Elm_Object_Item *li;
   char buf[256];

   EINA_SAFETY_ON_NULL_RETURN(ui.user_accounts.list);
   EINA_SAFETY_ON_NULL_RETURN(user_accounts);

   ecore_thread_main_loop_begin();
   EINA_LIST_FREE(user_accounts, acc)
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
        printf("Fuege Benutzer %s hinzu.\n", buf);
        li = elm_list_item_append(ui.user_accounts.list, buf, NULL, NULL, _on_user_account_select, NULL);
        elm_object_item_data_set(li, acc);
     }
   elm_list_go(ui.user_accounts.list);
   ecore_thread_main_loop_end();
}

Evas_Object *user_accounts_page_list_add(void)
{
   if (!ui.win) return NULL;

   ui.user_accounts.list = elm_list_add(ui.win);
   elm_list_multi_select_set(ui.user_accounts.list, EINA_TRUE);

   // Add callback for doubleclick action
   //evas_object_smart_callback_add(ui.user_accounts.enp.content, "clicked,double", _on_user_account_double_click, NULL);
   evas_object_smart_callback_add(ui.user_accounts.list, "clicked,double", _on_user_account_double_click, NULL);

   return ui.user_accounts.list;
}
