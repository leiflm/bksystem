#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Product.h"


void _products_list_reset(void)
{
   Elm_List_Item *eli;
   Elm_Gengrid_Item *egi;

   if ((eli = elm_list_selected_item_get(ui.products.list)))
     elm_list_item_selected_set(eli, EINA_FALSE);
}

static void
_on_product_select(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   if (!product) return;

   products_product_select(product);
}

static void
_products_fill(void)
{
   Evas_Object *ic;
   Eina_List *l;
   Elm_List_Item *li;
   Bks_Model_Product *product;
   char buf[256];

   if (!ui || !ui->products.list || !ui->model->products) return;

   EINA_LIST_FOREACH(ui->model->products, l, product)
     {
        ic = elm_icon_add(ui.products.list);
        snprintf(buf, sizeof(buf), "%s/%llu.png", elm_app_data_dir_get(), product->ean);
        elm_icon_file_set(ic, buf, NULL);
        elm_icon_scale_set(ic, 1, 1);
        li = elm_list_item_append(ui.products.list, product->name, ic, NULL, _on_product_select, product);
     }

   printf(elm_app_data_dir_get());
}


Evas_Object *_products_list_add(void)
{
   if (!ui || !ui->win) return NULL;

   ui.products.list = elm_list_add(ui->win);
   _products_fill();
   elm_list_go(ui.products.list);

   return ui.products.list;
}

