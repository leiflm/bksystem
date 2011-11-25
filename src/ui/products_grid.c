#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "UI.h"
#include "Product.h"

extern BKSystem_UI ui;
extern Eina_List *products;

static Elm_Gengrid_Item_Class gic;

void _products_grid_reset(void)
{
   Elm_List_Item *eli;
   Elm_Gengrid_Item *egi;

   if ((egi = elm_gengrid_selected_item_get(ui.products.grid)))
     elm_gen_item_selected_set(egi, EINA_FALSE);
}

static void
grid_selected(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Product *product = (Bks_Product*)data;

   products_product_selected(product);
}

static void
grid_longpress(void *data, Evas_Object *obj, void *event_info)
{
   printf("longpress %p\n", data);
}

char *
grid_label_get(void *data, Evas_Object *obj, const char *part)
{
   const Bks_Product *product = (Product*)data;
   char buf[256];

   if (!product) return NULL;

   snprintf(buf, sizeof(buf), "%s, %.2fEuro", product->name, product->price);
   return strdup(buf);
}

Evas_Object *
grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Product *product = (Product*)data;
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

Evas_Object *_products_grid_add(void)
{
   Eina_List *iter;
   Bks_Product *product;
   Elm_Gengrid_Item *li;

   if (!ui || !ui->win || !ui->model->products) return NULL;

   ui.products.grid = elm_gengrid_add(ui->win);
   elm_gengrid_item_size_set(ui.products.grid, 150, 150);
   evas_object_size_hint_weight_set(ui.products.grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   gic.item_style = "default";
   gic.func.label_get = grid_label_get;
   gic.func.content_get = grid_content_get;
   gic.func.state_get = grid_state_get;
   gic.func.del = grid_del;


   EINA_LIST_FOREACH(ui->model->products, iter, product)
      li = elm_gengrid_item_append(ui.products.grid, &gic, product, grid_selected, product);

   return ui.products.grid;
}

