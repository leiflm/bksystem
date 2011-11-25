#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include <Evas.h>

extern Evas_Object *_user_accounts_page_list_add();

static void
_on_user_accounts_prev_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   if (!ui) return;

   products_page_reset();
   elm_naviframe_item_promote(ui->productslist.enp.eoi);
}

static void
_on_user_accounts_finish_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   if (!ui) return;

   bks_sale_finish();
   clear_elm_list_selection(ui->productslist.list);
   elm_naviframe_item_promote(ui->productslist.enp.eoi);
}

void
user_accounts_page_reset(void)
{
   if (!ui) return NULL;

   clear_elm_list_selection(ui.accountslist.list);
   elm_widget_disabled_set(ui.accountslist.enp.next_btn, EINA_TRUE);
}

   void
user_accounts_page_add(void)
{
   if (!ui || !ui->win) return NULL;

   //create, setup and fill user_accounts
   ui->user_accounts.list = _user_accounts_page_list_add();
   // Add button to go back to productslist
   ui->user_accounts.enp.prev_btn = elm_button_add(ui->naviframe);
   evas_object_show(ui->user_accounts.enp.prev_btn);
   elm_object_text_set(ui->user_accounts.enp.prev_btn, "Zurück");
   evas_object_smart_callback_add(ui->user_accounts.enp.prev_btn, "clicked", _on_user_accounts_prev_btn_click, NULL);
   // Add button to finish shopping
   ui->user_accounts.enp.next_btn = elm_button_add(ui->naviframe);
   evas_object_show(ui->user_accounts.enp.next_btn);
   elm_object_text_set(ui->user_accounts.enp.next_btn, "Fertig");
   evas_object_smart_callback_add(ui->user_accounts.enp.next_btn, "clicked", _on_user_accounts_finish_btn_click, NULL);

   ui->user_accounts.enp.content = ui->user_accounts.list;
   ui->user_accounts.enp.eoi = elm_naviframe_item_push(ui->naviframe, "Konten", ui->user_accounts.enp.prev_btn, ui->user_accounts.enp.next_btn, ui->user_accounts.enp.content, NULL);
}
