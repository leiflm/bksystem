#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Ui_Private.h"

static void
_product_selected_del_cb(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
    free(data);
}

Evas_Object *_products_selected_list_add(void)
{
    Evas_Object *list = elm_list_add(ui.win);
    elm_win_resize_object_add(ui.win, list);
    elm_list_horizontal_set(list, EINA_TRUE);

    return list;
}

void _products_selected_list_clear(void)
{
   ecore_thread_main_loop_begin();
   elm_list_clear(ui.products.selected);
   ecore_thread_main_loop_end();
}

void products_selected_list_reset()
{
   if (!ui.products.selected) return;
   ecore_thread_main_loop_begin();
   elm_list_clear(ui.products.selected);
   ecore_thread_main_loop_end();
}

inline static void
_list_item_set_qty(Elm_Object_Item *eoi, Bks_Ui_Product_Selected *ps)
{
   char buf[256];

   snprintf(buf, (sizeof(buf) - 1), "%u %s", ps->qty, ps->product->name);
   elm_object_item_text_set(eoi, buf);
}

static void
_on_product_selected_click(void *data, Evas_Object *obj UNUSED, void *event_info)
{
   Elm_Object_Item *eoi = (Elm_Object_Item*)event_info;
   Bks_Ui_Product_Selected *ps = (Bks_Ui_Product_Selected*)data;

   if (--ps->qty == 0)
   {
       // remove from list
       elm_object_item_del(eoi);
       // If only the smart object is left in the list, disable the button
       if (eina_list_count(elm_list_items_get(ui.products.selected)) == 1)
           elm_object_disabled_set(ui.products.enp.next_btn, EINA_TRUE);
       return;
   }
   _list_item_set_qty(eoi, ps);

   elm_list_item_selected_set(eoi, EINA_FALSE);
}

void _products_selected_list_product_add(const Bks_Model_Product *product)
{
   Bks_Ui_Product_Selected *ui_product = NULL;
   Elm_Object_Item *eoi;
   Eina_List *iter;
   const Eina_List *items;

   ecore_thread_main_loop_begin();

   items = elm_list_items_get(ui.products.selected);

   elm_object_disabled_set(ui.products.enp.next_btn, EINA_FALSE);

   EINA_LIST_FOREACH((Eina_List*)items, iter, eoi)
   {
       ui_product = (Bks_Ui_Product_Selected*)elm_object_item_data_get(eoi);
       if (ui_product->product->pid == product->pid)
           break;
       ui_product = NULL;
   }

   if (!ui_product)
   {
       //product is not yet in the list
       ui_product = malloc(sizeof(Bks_Ui_Product_Selected));
       ui_product->qty = 0;
       ui_product->product = product;

       eoi = elm_list_item_append(ui.products.selected, NULL, NULL, NULL, _on_product_selected_click, ui_product);
       elm_object_item_del_cb_set(eoi, _product_selected_del_cb);
   }

   ui_product->qty++;
   _list_item_set_qty(eoi, ui_product);

   elm_list_go(ui.products.selected);
   ecore_thread_main_loop_end();
}

Eina_List *_products_selected_list_products_get(void)
{
   Bks_Ui_Product_Selected *ui_product = NULL;
   unsigned int i = 0;
   const Eina_List *items;
   Eina_List *iter, *products = NULL;
   Elm_Object_Item *eoi;

   ecore_thread_main_loop_begin();

   items = elm_list_items_get(ui.products.selected);
   EINA_LIST_FOREACH((Eina_List*)items, iter, eoi)
   {
       if (!(ui_product = (Bks_Ui_Product_Selected*)elm_object_item_data_get(eoi)))
           continue;
       for (i = 0; i < ui_product->qty; i++)
       {
           products = eina_list_append(products, (const void*)ui_product->product);
       }
   }

   ecore_thread_main_loop_end();

   return products;
}
