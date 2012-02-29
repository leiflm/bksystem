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

void user_accounts_page_fill(const Eina_List *user_accounts)
{
   user_accounts_list_fill(user_accounts);
}
