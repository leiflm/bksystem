#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

static void
_on_user_account_select(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   const Bks_Model_User_Account *acc = NULL;

   acc = (const Bks_Model_User_Account*)data;
   EINA_SAFETY_ON_NULL_RETURN(acc);

   printf("Account %s, %s was selected\n", acc->lastname, acc->firstname);

   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_FALSE);
}

static void
_del_cb(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   Bks_Model_User_Account *acc = NULL;

   acc = (Bks_Model_User_Account*)data;
   EINA_SAFETY_ON_NULL_RETURN(acc);

   bks_model_user_account_free(acc);
}

/*
static void
_on_user_account_double_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   //Only react if only a single account is selected
   if (eina_list_count(elm_list_selected_items_get(ui.user_accounts.enp.content)) > 1)
     return;
   bks_controller_ui_sale_finish();
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
}
*/

void
user_accounts_list_set(Eina_List *user_accounts)
{
   Bks_Model_User_Account *acc;
   Elm_Object_Item *li;
   Evas_Object *ic = NULL;
   char buf[256];
   const char *img_name = NULL;

   EINA_SAFETY_ON_NULL_RETURN(ui.user_accounts.list);
   EINA_SAFETY_ON_NULL_RETURN(user_accounts);

   ecore_thread_main_loop_begin();
   EINA_LIST_FREE(user_accounts, acc)
     {
        switch (acc->status)
          {
           case 1:
              img_name = "locked";
              break;
           default:
              img_name = NULL;
              break;
          }
        if (!img_name)
          ic = NULL;
        else
          {
             ic = elm_icon_add(ui.user_accounts.list);
             //snprintf(buf, (sizeof(buf) - 1), "%s/images/accounts/%s.png", elm_app_data_dir_get(), img_name);
             snprintf(buf, (sizeof(buf) - 1), "images/accounts/%s.png", img_name);
             elm_icon_standard_set(ic, buf);
             elm_image_resizable_set(ic, 0.0, 0.0);

             printf("Trying to open %s as icon\n", buf);
          }
        snprintf(buf, (sizeof(buf) - 1), "%s, %s", acc->lastname, acc->firstname);
        li = elm_list_item_append(ui.user_accounts.list, buf, NULL, ic, _on_user_account_select, acc);
        elm_object_item_del_cb_set(li, _del_cb);
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
   //evas_object_smart_callback_add(ui.user_accounts.list, "clicked,double", _on_user_account_double_click, NULL);

   return ui.user_accounts.list;
}
