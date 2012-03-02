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
   const Elm_Object_Item *selected_product = (const Elm_Object_Item*)event_info;

   _product_selected_set(selected_product);

   printf("Produkt %p ausgewaehlt.\n", selected_product);
}

void
products_list_set(Eina_List *products)
{
   Evas_Object *ic;
   Elm_Object_Item *it;
   Bks_Model_Product *product;
   char buf[256];

   EINA_SAFETY_ON_NULL_RETURN(ui.products.list);
   EINA_SAFETY_ON_NULL_RETURN(products);

   EINA_LIST_FREE(products, product)
     {
        ic = elm_icon_add(ui.products.list);
        snprintf(buf, sizeof(buf), "%s/%llu.png", "data", product->EAN);
        if (elm_icon_file_set(ic, buf, NULL))
          elm_icon_scale_set(ic, 1, 1);
        it = elm_list_item_append(ui.products.list, product->name, ic, NULL, _on_product_select, NULL);
        elm_object_item_data_set(it, product);
        printf("Produkt %p hinzugefuegt.\n", it);
     }

   elm_list_go(ui.products.list);
}

Evas_Object *products_list_add(void)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ui.win, NULL);

   ui.products.list = elm_list_add(ui.win);

   return ui.products.list;
}

