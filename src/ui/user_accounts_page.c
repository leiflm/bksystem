#include <Evas.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

static void
_on_user_accounts_prev_btn_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   products_page_reset();
   //The line below is necessary since the HIDE event callback for the user list isn't called
   evas_object_hide(ui.user_accounts.index);
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

static void
_on_user_accounts_finish_btn_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   bks_controller_ui_sale_finish();
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
   //The line below is necessary since the HIDE event callback for the user list isn't called
   evas_object_hide(ui.user_accounts.index);
}

void
user_accounts_page_reset(void)
{
   ecore_thread_main_loop_begin();
   elm_list_selection_clear(ui.user_accounts.list);
   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_TRUE);
   //The line below is necessary since the SHOW event callback for the user list isn't called
   evas_object_show(ui.user_accounts.index);
   ecore_thread_main_loop_end();
}

   Evas_Object
*user_accounts_page_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   //create, setup and fill user_accounts
   ui.user_accounts.list = user_accounts_page_list_add();
   EXPAND_AND_FILL(ui.user_accounts.list);
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

   ui.user_accounts.lock_window.win = elm_win_inwin_add(ui.win);
   ui.user_accounts.lock_window.content = elm_label_add(ui.user_accounts.lock_window.win);
   evas_object_size_hint_align_set(ui.user_accounts.lock_window.content, 0.5, 0.5);
   elm_object_text_set(ui.user_accounts.lock_window.content, "Die Benutzerkontenliste wird aktualisiert");
   elm_win_inwin_content_set(ui.user_accounts.lock_window.win, ui.user_accounts.lock_window.content);

   return ui.user_accounts.list;
}

void user_accounts_page_set(Eina_List *user_accounts)
{
   user_accounts_list_set(user_accounts);
}

// API calls

/**
 * @brief Clears the user accounts ui elements.
 */
void bks_ui_controller_user_accounts_clear(void)
{
   ecore_thread_main_loop_begin();
   elm_list_clear(ui.user_accounts.list);
   elm_index_item_clear(ui.user_accounts.index);
   ecore_thread_main_loop_end();
}

void bks_ui_controller_user_accounts_page_set(Eina_List *user_accounts)
{
   EINA_SAFETY_ON_NULL_RETURN(user_accounts);

   user_accounts_page_set(user_accounts);
}

/**
 * @return List of Bks_Model_User_Account elements
 */
Eina_List *bks_ui_controller_user_accounts_selected_get(void)
{
   const Eina_List *selected_accounts;
   Eina_List *iter, *list = NULL;
   Elm_Object_Item *eoi;
   const Bks_Model_User_Account *acc;

   ecore_thread_main_loop_begin();
   if (!(selected_accounts = elm_list_selected_items_get(ui.user_accounts.list)))
     {
        goto _user_accounts_selected_get_exit;
     }

   // print the names of all selected accounts
   EINA_LIST_FOREACH((Eina_List*)selected_accounts, iter, eoi)
     {
        if (elm_list_item_selected_get(eoi))
          {
             acc = (Bks_Model_User_Account*)elm_object_item_data_get(eoi);
#ifdef DEBUG
             printf("Selected User Account: %s, %s\n", acc->lastname, acc->firstname);
#endif
             list = eina_list_append(list, acc);
          }
     }

_user_accounts_selected_get_exit:
   ecore_thread_main_loop_end();
   return list;
}
