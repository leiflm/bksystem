#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Types.h"
#include "Bks_Model_Product.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"

void _products_list_reset
{
   Elm_List_Item *eli;

   if ((eli = elm_list_selected_item_get(ui.products.list)))
     elm_list_item_selected_set(eli, EINA_FALSE);
}

static void
_on_product_select(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Ui *ui = (Bks_Ui*)event_info;
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   if (!ui || !product) return;

   products_product_selected(ui, product);
}

static void
_products_list_fill(const Bks_Model *model)
{
   Evas_Object *ic;
   Eina_List *l;
   Bks_Model_Product *product;
   char buf[256];

   if (!ui || !ui.products.list || !model || !mdl.products) return;

   EINA_LIST_FOREACH(mdl.products, l, product)
     {
        ic = elm_icon_add(ui.products.list);
        snprintf(buf, sizeof(buf), "%s/%llu.png", elm_app_data_dir_get(), product->ean);
        elm_icon_file_set(ic, buf, NULL);
        elm_icon_scale_set(ic, 1, 1);
        elm_list_item_append(ui.products.list, product->name, ic, NULL, _on_product_select, product);
     }

   printf(elm_app_data_dir_get());

   elm_list_go(ui.products.list);
}


Evas_Object *_products_list_add
{
   if (!ui || !ui.win) return NULL;

   ui.products.list = elm_list_add(ui.win);

   return ui.products.list;
}

