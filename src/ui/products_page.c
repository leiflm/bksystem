#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Thread_Queue.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void _async_products_page_alpha_clear(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Thread_Queue_Element *tqe = (Bks_Thread_Queue_Element *)data;

   bks_thread_queue_wait(tqe);
   _products_grid_clear();
   bks_thread_queue_wake_up_next(tqe);
}

void _async_products_page_favs_clear(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Thread_Queue_Element *tqe = (Bks_Thread_Queue_Element *)data;

   bks_thread_queue_wait(tqe);
   _products_list_clear();
   bks_thread_queue_wake_up_next(tqe);
}

void _product_selected_set(const Elm_Object_Item *product)
{
   if (!product) return;

   ui.products.selected = product;
   user_accounts_page_reset();
   ecore_thread_main_loop_begin();
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
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
   EXPAND_AND_FILL(ui.products.panes);

   ui.products.favs = _products_favs_add();
   EXPAND_AND_FILL(ui.products.favs);
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui.products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add();
   EXPAND_AND_FILL(ui.products.alpha);
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui.products.panes, "right", ui.products.alpha);

   ui.products.lock_window.win = elm_win_inwin_add(ui.win);
   ui.products.lock_window.content = elm_label_add(ui.products.lock_window.win);
   evas_object_size_hint_align_set(ui.products.lock_window.content, 0.5, 0.5);
   elm_object_text_set(ui.products.lock_window.content, "Die Produktliste wird aktualisiert");
   elm_win_inwin_content_set(ui.products.lock_window.win, ui.products.lock_window.content);

   return ui.products.panes;
}

void _async_favs_set(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Thread_Queue_Element *tqe = (Bks_Thread_Queue_Element *)data;
   Eina_List *products = NULL;

   bks_thread_queue_wait(tqe);

   EINA_SAFETY_ON_NULL_RETURN(tqe->data);
   products = (Eina_List*)(tqe->data);

   products_grid_set(products);
   bks_ui_products_favs_update_set(EINA_FALSE);
   bks_thread_queue_wake_up_next(tqe);
}

void _async_alpha_set(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Thread_Queue_Element *tqe = (Bks_Thread_Queue_Element *)data;
   Eina_List *products = NULL;

   bks_thread_queue_wait(tqe);

   EINA_SAFETY_ON_NULL_RETURN(tqe->data);
   products = (Eina_List*)(tqe->data);

   products_list_set(products);
   bks_ui_products_alpha_update_set(EINA_FALSE);
   bks_thread_queue_wake_up_next(tqe);
}

const Bks_Model_Product *bks_ui_product_selected_get(void)
{
   const Bks_Model_Product *product;

   product = (Bks_Model_Product*)elm_object_item_data_get(ui.products.selected);

   return product;
}

//API calls

void bks_ui_products_alpha_clear(const Bks_Thread_Queue_Element *element)
{
   ecore_thread_run(_async_products_page_alpha_clear, NULL, NULL, (void*)element);
}

void bks_ui_products_favs_clear(const Bks_Thread_Queue_Element *element)
{
   ecore_thread_run(_async_products_page_favs_clear, NULL, NULL, (void*)element);
}

void bks_ui_products_alpha_update_set(const Eina_Bool update)
{
   if (!update)
     {
        ecore_thread_main_loop_begin();
        evas_object_hide(ui.products.lock_window.win);
        ecore_thread_main_loop_end();
        return;
     }

   if (!evas_object_visible_get(ui.lock_window.win))
     {
        ecore_thread_main_loop_begin();
        elm_win_inwin_activate(ui.products.lock_window.win);
        ecore_thread_main_loop_end();
     }
}

void bks_ui_products_favs_update_set(const Eina_Bool update)
{
   if (!update)
     {
        ecore_thread_main_loop_begin();
        evas_object_hide(ui.products.lock_window.win);
        ecore_thread_main_loop_end();
        return;
     }

   if (!evas_object_visible_get(ui.lock_window.win))
     {
        ecore_thread_main_loop_begin();
        elm_win_inwin_activate(ui.products.lock_window.win);
        ecore_thread_main_loop_end();
     }
}

/**
 * @brief fills the UI's favourite's section using @p products.
 *
 * @param products list of Bks_Model_Product
 */
void bks_ui_products_page_favs_set(const Bks_Thread_Queue_Element *element)
{
   ecore_thread_run(_async_favs_set, NULL, NULL, (void*)element);
}

void bks_ui_products_page_alpha_set(const Bks_Thread_Queue_Element *element)
{
   ecore_thread_run(_async_alpha_set, NULL, NULL, (void*)element);
}
