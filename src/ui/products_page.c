#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Elm_Utils.h"
#include <Elementary.h>

void products_product_selected(Bks_Model_Product *product)
{
   if (!product) return;

   printf("Product %s was selected\n", product->name);
   user_accounts_page_reset(ui);
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
}

/**
 * @brief This function resets the product selection. It does not reload the
 * data.
 */
void
products_page_reset(Evas_Object *products_page)
{
   products_grid_reset(products_page);
   products_list_reset(products_page);
}

Evas_Object *_products_favs_add
{
   return products_grid_add(ui);
}

Evas_Object *_products_alpha_add
{
   return products_list_add(ui);
}

Evas_Object *products_page_add
{
   EINA_SAFETY_ON_TRUE_RETURN_VAL((!ui || !ui.win), NULL);

   ui.products.panes = elm_panes_add(ui.win);
   elm_panes_horizontal_set(ui.products.panes, EINA_TRUE);

   ui.products.favs = _products_favs_add(ui);
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui.products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add(ui);
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui.products.panes, "right", ui.products.alpha);

   return ui.products.panes;
}

void _products_page_favs_fill(const Bks_Model *model)
{
   products_grid_fill(ui, model);
}


void _products_page_alpha_fill(const Bks_Model *model)
{
   products_list_fill(ui, model);
}

void products_page_fill(const Bks_Model *model)
{
   _products_page_favs_fill(ui, model);
   _products_page_alpha_fill(ui, model);
}
