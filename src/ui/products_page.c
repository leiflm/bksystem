#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void _async_products_page_clear(void *data)
//void _async_products_page_clear(void *data, Ecore_Thread *th)
{
   _products_grid_clear();
   _products_list_clear();
}

void _product_selected_set(const Elm_Object_Item *product)
{
   if (!product) return;

   ui.products.selected = product;
   user_accounts_page_reset();
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
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
}

Evas_Object *_products_favs_add(void)
{
   return products_grid_add();
}

Evas_Object *_products_alpha_add(void)
{
   return products_list_add();
}

Evas_Object *products_page_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   ui.products.panes = elm_panes_add(ui.win);
   elm_panes_horizontal_set(ui.products.panes, EINA_TRUE);

   ui.products.favs = _products_favs_add();
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui.products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add();
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui.products.panes, "right", ui.products.alpha);

   ui.products.lock_window.win = elm_win_inwin_add(ui.win);
   ui.products.lock_window.content = elm_label_add(ui.products.lock_window.win);
   elm_object_text_set(ui.products.lock_window.content, "Die Produktliste wird aktualisiert");
   elm_win_inwin_content_set(ui.products.lock_window.win, ui.products.lock_window.content);

   return ui.products.panes;
}

//void _async_favs_set(void *data, Ecore_Thread *th)
void _async_favs_set(void *data)
{
   Eina_List *products = (Eina_List*)data;

   products_grid_set(products);
   bks_ui_products_update_set(EINA_FALSE);
}

//void _async_alpha_set(void *data, Ecore_Thread *th)
void _async_alpha_set(void *data)
{
   Eina_List *products = (Eina_List*)data;

   products_list_set(products);
   bks_ui_products_update_set(EINA_FALSE);
}

const Bks_Model_Product *bks_ui_product_selected_get(void)
{
   const Bks_Model_Product *product;

   product = (Bks_Model_Product*)elm_object_item_data_get(ui.products.selected);

   return product;
}

//API calls

/**
 * @brief Clears the products ui elements.
 */
void bks_ui_products_clear(void)
{
   printf("Produktseite wird geleert.\n");
   ecore_main_loop_thread_safe_call_async(_async_products_page_clear, NULL);
   //ecore_thread_run(_async_products_page_clear, NULL, NULL, NULL);
}

/**
 * @brief Indicates that the products are being refetched.
 */
void bks_ui_products_update_set(const Eina_Bool update)
{
   printf("Jetzt sollte der Produktbildschirm %sbenutzbar sein.\n", (update ? "un" : ""));

   if (!update)
     {
        evas_object_hide(ui.products.lock_window.win);
        return;
     }

   if (!evas_object_visible_get(ui.lock_window.win))
     elm_win_inwin_activate(ui.products.lock_window.win);
}

/**
 * @brief fills the UI's favourite's section using @p products.
 *
 * @param products list of Bks_Model_Product
 */
void bks_ui_products_page_favs_set(Eina_List *products)
{
   //ecore_thread_run(_async_favs_set, NULL, NULL, products);
   ecore_main_loop_thread_safe_call_async(_async_favs_set, products);
}

/**
 * @brief fills the UI's favourite's section using @p products.
 *
 * @param products list of Bks_Model_Product
 */
void bks_ui_products_page_alpha_set(Eina_List *products)
{
   //CREATE THREAD
   //ecore_thread_run(_async_alpha_set, NULL, NULL, products);
   ecore_main_loop_thread_safe_call_async(_async_alpha_set, products);
}
