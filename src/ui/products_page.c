#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void _products_selected_product_add(const Bks_Model_Product *product)
{
   if (!product) return;

   ecore_thread_main_loop_begin();
   _products_selected_list_product_add(product);
   ecore_thread_main_loop_end();
}

/**
 * @brief This function resets the product selection. It does not reload the
 * data.
 */
void
products_page_reset(void)
{
   products_grid_reset();
   products_list_reset();
   products_selected_list_reset();
}

Evas_Object *_products_favs_add(void)
{
   return products_grid_add();
}

Evas_Object *_products_alpha_add(void)
{
   return products_list_add();
}

static void
_on_products_next_btn_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   bks_controller_ui_sale_finish();
   user_accounts_page_reset();
   products_page_reset();
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
}

static void
_on_products_prev_btn_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
}

Evas_Object *products_page_add(void)
{
   Evas_Object *tb;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   tb = elm_table_add(ui.win);
   elm_win_resize_object_add(ui.win, tb);

   ui.products.favs = _products_favs_add();
   evas_object_show(ui.products.favs);
   evas_object_size_hint_weight_set(ui.products.favs, 0.7, 0.8);
   FILL(ui.products.favs);
   elm_table_pack(tb, ui.products.favs, 0, 0, 1, 1);

   ui.products.alpha = _products_alpha_add();
   evas_object_show(ui.products.alpha);
   evas_object_size_hint_weight_set(ui.products.alpha, 0.3, 0.8);
   FILL(ui.products.alpha);
   elm_table_pack(tb, ui.products.alpha, 1, 0, 1, 1);

   ui.products.selected_scroller = _products_selected_list_add();
   evas_object_show(ui.products.selected_scroller);
   evas_object_size_hint_weight_set(ui.products.selected_scroller, EVAS_HINT_EXPAND, 0.2);
   FILL(ui.products.selected_scroller);
   elm_table_pack(tb, ui.products.selected_scroller, 0, 1, 2, 1);

   // Add button to switch back to account selection
   ui.products.enp.prev_btn = elm_button_add(ui.naviframe);
   evas_object_show(ui.products.enp.prev_btn);
   elm_object_text_set(ui.products.enp.prev_btn, "Zurück");
   evas_object_smart_callback_add(ui.products.enp.prev_btn, "clicked", _on_products_prev_btn_click, NULL);

   // Add button to continue to account selection
   ui.products.enp.next_btn = elm_button_add(ui.naviframe);
   elm_object_disabled_set(ui.products.enp.next_btn, EINA_TRUE);
   evas_object_show(ui.products.enp.next_btn);
   elm_object_text_set(ui.products.enp.next_btn, "Kauf abschließen");
   evas_object_smart_callback_add(ui.products.enp.next_btn, "clicked", _on_products_next_btn_click, NULL);

   ui.products.lock_window.win = elm_win_inwin_add(ui.win);
   ui.products.lock_window.content = elm_label_add(ui.products.lock_window.win);
   evas_object_size_hint_align_set(ui.products.lock_window.content, 0.5, 0.5);
   elm_object_text_set(ui.products.lock_window.content, "Die Produktliste wird aktualisiert");
   elm_win_inwin_content_set(ui.products.lock_window.win, ui.products.lock_window.content);

   return tb;
}

Eina_List *bks_ui_controller_products_selected_get(void)
{
   return _products_selected_list_products_get();
}

//API calls

void bks_ui_controller_products_alpha_clear(void)
{
    _products_grid_clear();
}

void bks_ui_controller_products_favs_clear(void)
{
   _products_list_clear();
}


void bks_ui_controller_products_selected_clear(void)
{
    //_products_selected_clear();
}

/**
 * @brief fills the UI's favourite's section using @p products.
 *
 * @param products list of Bks_Model_Product
 */
void bks_ui_controller_products_page_favs_set(Eina_List *products)
{
   products_grid_set(products);
}

void bks_ui_controller_products_page_alpha_set(Eina_List *products)
{
   products_list_set(products);
}
