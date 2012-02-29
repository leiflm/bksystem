#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void products_page_clear(void)
{
   elm_gengrid_clear(ui.products.grid);
   elm_list_clear(ui.products.list);
}

void products_product_selected(Bks_Model_Product *product)
{
   if (!product) return;

   printf("Product %s was selected\n", product->name);
   user_accounts_page_reset();
   elm_naviframe_item_promote(ui.user_accounts.enp.eoi);
}

/**
 * @brief This function resets the product selection. It does not reload the
 * data.
 */
void
products_page_reset(void)
{
   products_grid_reset();
   products_list_reset();
}

Evas_Object *_products_favs_add(void)
{
   return products_grid_add();
}

Evas_Object *_products_alpha_add(void)
{
   return products_list_add();
}

Evas_Object *products_page_add(void)
{
   EINA_SAFETY_ON_TRUE_RETURN_VAL((!ui.win), NULL);

   ui.products.panes = elm_panes_add(ui.win);
   elm_panes_horizontal_set(ui.products.panes, EINA_TRUE);

   ui.products.favs = _products_favs_add();
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui.products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add();
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui.products.panes, "right", ui.products.alpha);

   return ui.products.panes;
}

void _products_page_favs_fill(const Eina_List *products)
{
   products_grid_fill(products);
}

void _products_page_alpha_fill(const Eina_List *products)
{
   products_list_fill(products);
}

void products_page_fill(const Eina_List *products)
{
   _products_page_favs_fill(products);
   _products_page_alpha_fill(products);
}
