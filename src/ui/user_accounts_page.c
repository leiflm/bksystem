#include <Evas.h>
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"

extern Evas_Object *_user_accounts_page_list_add();

static void
_on_user_accounts_prev_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Ui *ui = (Bks_Ui*)event_info;

   EINA_SAFETY_ON_NULL_RETURN(ui);

   products_page_reset();
   elm_naviframe_item_promote(ui.productslist.enp.eoi);
}

static void
_on_user_accounts_finish_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Ui *ui = (Bks_Ui*)event_info;

   EINA_SAFETY_ON_NULL_RETURN(ui);

   bks_sale_finish();
   clear_elm_list_selection(ui.productslist.list);
   elm_naviframe_item_promote(ui.productslist.enp.eoi);
}

void
user_accounts_page_reset
{
   if (!ui) return;

   clear_elm_list_selection(ui.accountslist.list);
   elm_widget_disabled_set(ui.accountslist.enp.next_btn, EINA_TRUE);
}

   Evas_Object
*user_accounts_page_add(void)
{
   if (!ui || !ui.win) return NULL;

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

Evas_Object *user_accounts_page_fill(const Bks_Model *model)
{
   user_accounts_list_fill(ui, model);
}
