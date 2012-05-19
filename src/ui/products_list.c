#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Model_Product.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"

static void _product_obj_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);

void _products_list_clear()
{
   ecore_thread_main_loop_begin();
   elm_list_clear(ui.products.list);
   ecore_thread_main_loop_end();
}

void products_list_reset(void)
{
   Elm_Object_Item *eoi;

   ecore_thread_main_loop_begin();
   if ((eoi = elm_list_selected_item_get(ui.products.list)))
     elm_list_item_selected_set(eoi, EINA_FALSE);
   ecore_thread_main_loop_end();
}

static void
_on_product_select(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info)
{
   const Elm_Object_Item *selected_product = (const Elm_Object_Item*)event_info;

   _product_selected_set(selected_product);

   printf("Produkt %p ausgewaehlt.\n", selected_product);
}

void
products_list_set(Eina_List *products)
{
   Evas_Object *icon = NULL, *lbl = NULL;
   Elm_Object_Item *it;
   Bks_Model_Product *product;
   static Evas *evas = NULL;
   static char buf[128];

   EINA_SAFETY_ON_NULL_RETURN(ui.products.list);

   ecore_thread_main_loop_begin();
   if (!evas)
     evas = evas_object_evas_get(ui.products.list);

   if (!products)
     elm_list_item_append(ui.products.list, "Keine Produkte in der Datenbank gefunden.", NULL, NULL, NULL, NULL);

   EINA_LIST_FREE(products, product)
     {
        it = NULL;
        icon = lbl = NULL;
        if (product->image.data)
          {
             icon = evas_object_image_filled_add(evas);
             evas_object_image_memfile_set(icon, product->image.data, product->image.size, NULL, NULL);
             //evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
             evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_show(icon);
          }
        lbl = elm_label_add(ui.products.list);
        snprintf(buf, (sizeof(buf) - 1), "%.2f Euro", product->price);
        evas_object_event_callback_add(lbl, EVAS_CALLBACK_DEL, _product_obj_del_cb, product);
        elm_object_text_set(lbl, buf);
        evas_object_show(lbl);
        it = elm_list_item_append(ui.products.list, product->name, icon, lbl, _on_product_select, NULL);
        elm_object_item_data_set(it, product);
     }
   elm_list_go(ui.products.list);
   ecore_thread_main_loop_end();
}

Evas_Object *products_list_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   ui.products.list = elm_list_add(ui.win);

   return ui.products.list;
}

static void
_product_obj_del_cb(void *data, Evas *e UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   if (!product)
     return;

   bks_model_product_free(product);
}
