#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"

void _products_grid_clear()
{
   ecore_thread_main_loop_begin();
   elm_gengrid_clear(ui.products.grid);
   ecore_thread_main_loop_end();
}

void products_grid_reset()
{
   if (!ui.products.grid) return;
   ecore_thread_main_loop_begin();
   elm_gengrid_selection_clear(ui.products.grid);
   ecore_thread_main_loop_end();
}

static void
grid_selected(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info)
{
   const Elm_Object_Item *selected_product = (const Elm_Object_Item*)event_info;

   _product_selected_set(selected_product);

   printf("Produkt %p ausgewaehlt.\n", selected_product);
}

char *
grid_text_get(void *data, Evas_Object *obj UNUSED, const char *part UNUSED)
{
   const Bks_Model_Product *product = (Bks_Model_Product*)data;
   char buf[32];

   if (product)
     snprintf(buf, (sizeof(buf) - 1), "%s, %.2f Euro", product->name, product->price);
   else
     snprintf(buf, (sizeof(buf) - 1), "Keine Favoriten vorhanden");

   return strdup(buf);
}

Evas_Object *
grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Bks_Model_Product *product = (Bks_Model_Product*)data;
   Evas_Object *icon = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        if (product && product->image.data)
          {
             icon = evas_object_image_filled_add(evas_object_evas_get(obj));
             evas_object_image_memfile_set(icon, product->image.data, product->image.size, NULL, NULL);
             evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
             evas_object_show(icon);
          }
        return icon;
     }
   /*
    * Do not add a check indicator for now
    else if (!strcmp(part, "elm.swallow.end"))
    {
    Evas_Object *ck;
    ck = elm_check_add(obj);
    evas_object_propagate_events_set(ck, 0);
    elm_check_state_set(ck, ti->onoff);
    evas_object_smart_callback_add(ck, "changed", grid_item_check_changed, data);
    evas_object_show(ck);
    return ck;
    }
    */

   return NULL;
}

   Eina_Bool
grid_state_get(void *data UNUSED, Evas_Object *obj UNUSED, const char *part UNUSED)
{
   return EINA_FALSE;
}

   void
grid_del(void *data, Evas_Object *obj UNUSED)
{
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   if (!product)
     return;

   bks_model_product_free(product);
}

/*
static void
grid_item_check_changed(void *data, Evas_Object *obj, void *event_info)
{
   Testitem *ti = data;
   ti->onoff = elm_check_state_get(obj);
   printf("item %p onoff = %i\n", ti, ti->onoff);
}
*/

Evas_Object *products_grid_add(void)
{
   ui.products.grid = elm_gengrid_add(ui.win);
   elm_gengrid_item_size_set(ui.products.grid, 150, 150);
   evas_object_size_hint_weight_set(ui.products.grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return ui.products.grid;
}

void products_grid_set(Eina_List *products)
{
   Bks_Model_Product *product;
   static Elm_Gengrid_Item_Class gic;

   EINA_SAFETY_ON_NULL_RETURN(ui.products.grid);

   // set callbacks for grid elements
   gic.item_style = "default";
   gic.func.text_get = grid_text_get;
   gic.func.content_get = grid_content_get;
   gic.func.state_get = grid_state_get;
   gic.func.del = grid_del;

   ecore_thread_main_loop_begin();

   if (!products) //Add fake element if none are present
     elm_gengrid_item_append(ui.products.grid, &gic, NULL, NULL, NULL);

   //add all products to grid
   EINA_LIST_FREE(products, product)
     {
        elm_gengrid_item_append(ui.products.grid, &gic, product, grid_selected, NULL);
     }
   ecore_thread_main_loop_end();
}
