#include <Evas.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

extern Evas_Object *_user_accounts_page_list_add();

void user_accounts_page_clear(void)
{
   elm_list_clear(ui.user_accounts.list);
}

static void
_on_user_accounts_prev_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

static void
_on_user_accounts_finish_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   bks_controller_ui_sale_finish_cb();
   elm_list_selection_clear(ui.products.list);
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

void
user_accounts_page_reset(void)
{
   elm_list_selection_clear(ui.user_accounts.list);
   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_TRUE);
}

   Evas_Object
*user_accounts_page_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   //create, setup and fill user_accounts
   ui.user_accounts.list = _user_accounts_page_list_add();
   // Add button to go back to productslist
   ui.user_accounts.enp.prev_btn = elm_button_add(ui.naviframe);
   evas_object_show(ui.user_accounts.enp.prev_btn);
   elm_object_text_set(ui.user_accounts.enp.prev_btn, "Zurück");
   evas_object_smart_callback_add(ui.user_accounts.enp.prev_btn, "clicked", _on_user_accounts_prev_btn_click, NULL);
   // Add button to finish shopping
   ui.user_accounts.enp.next_btn = elm_button_add(ui.naviframe);
   evas_object_show(ui.user_accounts.enp.next_btn);
   elm_object_text_set(ui.user_accounts.enp.next_btn, "Fertig");
   evas_object_smart_callback_add(ui.user_accounts.enp.next_btn, "clicked", _on_user_accounts_finish_btn_click, NULL);

   return ui.user_accounts.list;
}

void user_accounts_page_set(const Eina_List *user_accounts)
{
    user_accounts_list_set(user_accounts);
}

// API calls

/**
 * @brief Clears the user accounts ui elements.
 */
void bks_ui_user_accounts_clear(void)
{
   user_accounts_page_clear();
   eina_lock_release(&ui.user_accounts.lock);
}

/**
 * @brief Indicates that the user accounts are being refetched.
 */
void bks_ui_user_accounts_update_set(const Eina_Bool update)
{
    printf("Jetzt sollte der Benutzerlistenschirm  %sbenutzbar sein.\n", (update ? "un" : ""));
}

void _bks_ui_user_accounts_page_set(void *data, Ecore_Thread *th)
{
   const Eina_List *user_accounts = (const Eina_List*)data;

   user_accounts_page_set(user_accounts);
   eina_lock_release(&ui.user_accounts.lock);
}

void bks_ui_user_accounts_page_set(const Eina_List *user_accounts)
{
   bks_ui_user_accounts_lock_take();
   //CREATE THREAD
   ecore_thread_run(_bks_ui_user_accounts_page_set, NULL, NULL, user_accounts);
}

/**
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

/**
 * @brief Aquires a lock for the user accounts ui elements
 */
void bks_ui_user_accounts_lock_take(void)
{
    eina_lock_take(&ui.user_accounts.lock);
}
