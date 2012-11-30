#include <Evas.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

typedef enum {
    BKS_USER_ACCOUNTS_FILTER_FAVS,
    BKS_USER_ACCOUNTS_FILTER_ALL } Bks_Ui_Filter_Mode;

static void
_on_user_accounts_sort_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info)
{
    Elm_Object_Item *it = (Elm_Object_Item*)event_info;
    const char *btn_txt = elm_object_item_text_get(it);

    bks_ui_controller_user_accounts_clear();
    if (strncmp("Meist Kaufende", btn_txt, sizeof("Meist Kaufende")) == 0)
      {
        bks_controller_user_accounts_favs_get(25);
      }
    else
      {
        bks_controller_user_accounts_alpha_get();
      }
}

static void
_on_user_accounts_products_btn_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
}

void
user_accounts_page_reset(void)
{
   ecore_thread_main_loop_begin();
   elm_list_selection_clear(ui.user_accounts.list);
   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_TRUE);
   ecore_thread_main_loop_end();
}

   Evas_Object
*user_accounts_page_add(void)
{
   Evas_Object *tb = NULL, *sc;
   Elm_Object_Item *btn;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   tb = elm_table_add(ui.win);
   elm_win_resize_object_add(ui.win, tb);

   //create, setup and fill user_accounts
   ui.user_accounts.list = user_accounts_page_list_add();
   evas_object_show(ui.user_accounts.list);
   evas_object_size_hint_weight_set(ui.user_accounts.list, 1.0, 1.0);
   FILL(ui.user_accounts.list);
   elm_table_pack(tb, ui.user_accounts.list, 1, 1, 2, 1);

   elm_table_pack(tb, ui.user_accounts.index, 1, 1, 2, 1);
   evas_object_show(ui.user_accounts.index);

   sc = elm_segment_control_add(ui.win);
   evas_object_show(sc);
   evas_object_size_hint_weight_set(sc, 1.0, 0.0);
   evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, 1.0);
   elm_table_pack(tb, sc, 1, 2, 1, 1);

   btn = elm_segment_control_item_add(sc, NULL, "Meist Kaufende");
   elm_segment_control_item_selected_set(btn, EINA_TRUE);

   btn = elm_segment_control_item_add(sc, NULL, "Alle Konten");

   // Add the callback later, so it won't be called for the item selected earlier
   evas_object_smart_callback_add(sc, "changed", _on_user_accounts_sort_click, NULL);

   // Add button to select products
   ui.user_accounts.enp.next_btn = elm_button_add(ui.naviframe);
   evas_object_show(ui.user_accounts.enp.next_btn);
   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_TRUE);
   elm_object_text_set(ui.user_accounts.enp.next_btn, "Produkte auswaehlen");
   evas_object_smart_callback_add(ui.user_accounts.enp.next_btn, "clicked", _on_user_accounts_products_btn_click, NULL);

   ui.user_accounts.lock_window.win = elm_win_inwin_add(ui.win);
   ui.user_accounts.lock_window.content = elm_label_add(ui.user_accounts.lock_window.win);
   evas_object_size_hint_align_set(ui.user_accounts.lock_window.content, 0.5, 0.5);
   elm_object_text_set(ui.user_accounts.lock_window.content, "Die Benutzerkontenliste wird aktualisiert");
   elm_win_inwin_content_set(ui.user_accounts.lock_window.win, ui.user_accounts.lock_window.content);

   return tb;
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
