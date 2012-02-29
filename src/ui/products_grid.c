#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"

void products_grid_reset()
{
   if (!ui.products.grid) return;
   elm_gengrid_selection_clear(ui.products.grid);
}

static void
grid_selected(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   EINA_SAFETY_ON_NULL_RETURN(product);

   products_product_selected(product);
}

char *
grid_text_get(void *data, Evas_Object *obj, const char *part)
{
   const Bks_Model_Product *product = (Bks_Model_Product*)data;
   char buf[256];

   if (!product) return NULL;

   snprintf(buf, sizeof(buf), "%s, %.2fEuro", product->name, product->price);
   return strdup(buf);
}

Evas_Object *
grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Bks_Model_Product *product = (Bks_Model_Product*)data;
   char buf[256];

   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *icon = elm_bg_add(obj);
        snprintf(buf, sizeof(buf), "%s/data/images/%llu.jpg", elm_app_data_dir_get(), product->ean);
        elm_bg_file_set(icon, buf, NULL);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(icon);
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
grid_state_get(void *data, Evas_Object *obj, const char *part)
{
   return EINA_FALSE;
}

   void
grid_del(void *data, Evas_Object *obj)
{
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
   Evas_Object *grid;

   grid = elm_gengrid_add(ui.win);
   elm_gengrid_item_size_set(grid, 150, 150);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return grid;
}

void _products_grid_clear(void)
{
   EINA_SAFETY_ON_NULL_RETURN(ui.products.grid);

   elm_gengrid_clear(ui.products.grid);
}

void products_grid_fill(const Eina_List *products)
{
   Eina_List *iter;
   Bks_Model_Product *product;
   static Elm_Gengrid_Item_Class gic;

   EINA_SAFETY_ON_NULL_RETURN(ui.products.grid);

   // set callbacks for grid elements
   gic.item_style = "default";
   gic.func.text_get = grid_text_get;
   gic.func.content_get = grid_content_get;
   gic.func.state_get = grid_state_get;
   gic.func.del = grid_del;

   _products_grid_clear();

   //add all products to grid
   EINA_LIST_FOREACH((Eina_List*)products, iter, product)
      elm_gengrid_item_append(ui.products.grid, &gic, product, grid_selected, NULL);
}
