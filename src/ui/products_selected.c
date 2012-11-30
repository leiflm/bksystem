#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Ui_Private.h"

static void
_product_selected_del_cb(void *data, Evas_Object *obj, void *event_info UNUSED)
{
    free(data);
}

Evas_Object *_products_selected_list_add(void)
{
    Evas_Object *list, *scroller;

    list = elm_box_add(ui.win);
    ui.products.selected = list;
    evas_object_show(list);
    evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(list, 0.0, 0.5);
    elm_box_horizontal_set(list, EINA_TRUE);
    elm_box_padding_set(list, 5, 1);

    scroller = elm_scroller_add(ui.win);
    elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_OFF);
    elm_win_resize_object_add(ui.win, scroller);
    elm_object_content_set(scroller, list);

    return scroller;
}

void _products_selected_list_clear(void)
{
   ecore_thread_main_loop_begin();
   elm_box_clear(ui.products.selected);
   ecore_thread_main_loop_end();
}

void products_selected_list_reset()
{
   if (!ui.products.selected) return;
   ecore_thread_main_loop_begin();
   elm_box_clear(ui.products.selected);
   ecore_thread_main_loop_end();
}

inline static void
_list_item_set_qty(Evas_Object *eo, Bks_Ui_Product_Selected *ps)
{
   char buf[256];

   snprintf(buf, (sizeof(buf) - 1), "%u %s", ps->qty, ps->product->name);
   elm_object_text_set(eo, buf);
}

static void
_on_product_selected_click(void *data, Evas_Object *obj, void *event_info UNUSED)
{
   Bks_Ui_Product_Selected *ps = (Bks_Ui_Product_Selected*)data;
   Eina_List *items = elm_box_children_get(ui.products.selected);

   if (--ps->qty == 0)
   {
       // remove from list
       elm_box_unpack(ui.products.selected, obj);
       evas_object_del(obj);
       // If only the smart object is left in the list, disable the button
       if (eina_list_count(items) == 1)
           elm_object_disabled_set(ui.products.enp.next_btn, EINA_TRUE);
       eina_list_free(items);
       return;
   }
   _list_item_set_qty(obj, ps);
   eina_list_free(items);
}

void _products_selected_list_product_add(const Bks_Model_Product *product)
{
   Bks_Ui_Product_Selected *ui_product = NULL;
   Evas_Object *l = NULL;
   Eina_List *iter, *items;

   ecore_thread_main_loop_begin();

   items = elm_box_children_get(ui.products.selected);

   elm_object_disabled_set(ui.products.enp.next_btn, EINA_FALSE);

   EINA_LIST_FOREACH(items, iter, l)
   {
       ui_product = (Bks_Ui_Product_Selected*)evas_object_data_get(l, "ui_product");
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

       l = elm_button_add(ui.products.selected);
       evas_object_show(l);
       evas_object_data_set(l, "ui_product", ui_product);
       evas_object_smart_callback_add(l, "clicked", _on_product_selected_click, ui_product);
       evas_object_smart_callback_add(l, "delete,request", _product_selected_del_cb, ui_product);
       items = eina_list_append(items, ui_product);

       elm_box_pack_end(ui.products.selected, l);
   }

   ui_product->qty++;
   _list_item_set_qty(l, ui_product);

   eina_list_free(items);

   ecore_thread_main_loop_end();
}

Eina_List *_products_selected_list_products_get(void)
{
   Bks_Ui_Product_Selected *ui_product = NULL;
   unsigned int i = 0;
   Eina_List *items;
   Eina_List *iter, *products = NULL;
   Evas_Object *o;

   ecore_thread_main_loop_begin();

   items = elm_box_children_get(ui.products.selected);
   EINA_LIST_FOREACH(items, iter, o)
   {
       if (!(ui_product = (Bks_Ui_Product_Selected*)evas_object_data_get(o, "ui_product")))
           continue;
       for (i = 0; i < ui_product->qty; i++)
       {
           products = eina_list_append(products, (const void*)ui_product->product);
       }
   }
   eina_list_free(items);

   ecore_thread_main_loop_end();

   return products;
}
