#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Model_Product.h"
#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"

void products_list_reset(void)
{
   Elm_Object_Item *eoi;

   if ((eoi = elm_list_selected_item_get(ui.products.list)))
     elm_list_item_selected_set(eoi, EINA_FALSE);
}

static void
_on_product_select(void *data, Evas_Object *obj, void *event_info)
{
   Bks_Model_Product *product = (Bks_Model_Product*)data;

   if (!product) return;

   products_product_selected(product);

   printf("Product %s selected.\n", product->name);
}

void
products_list_fill(const Eina_List *products)
{
   Evas_Object *ic;
   Eina_List *l;
   Bks_Model_Product *product;
   char buf[256];

   EINA_SAFETY_ON_NULL_RETURN(ui.products.list);
   EINA_SAFETY_ON_NULL_RETURN(products);

   EINA_LIST_FOREACH((Eina_List*)products, l, product)
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

Evas_Object *products_list_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   ui.products.list = elm_list_add(ui.win);

   return ui.products.list;
}

