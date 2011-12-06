#include "Bks_Ui_Private.h"
#include "Bks_Ui.h"
#include "Elm_Utils.h"
#include <Elementary.h>

extern Bks_Ui *ui;

extern Evas_Object *_products_list_add(void);
extern void _products_list_reset(void);
extern Evas_Object *_products_grid_add(void);
extern void _products_grid_reset(void);

void products_product_selected(Bks_Model_Product *product)
{
   if (!product) return;

   printf("Product %s was selected\n", product->name;
   user_accounts_page_reset();
   elm_naviframe_item_promote(ui.accountslist.enp.eoi);
}

/**
 * @brief This function resets the product selection. It does not reload the
 * data.
 */
static void
products_page_reset(Evas_Object *products_page)
{
   _products_grid_reset(products_page);
   _products_list_reset(products_page);
}

Evas_Object *_products_favs_add(const Bks_Ui *ui)
{
   return _products_grid_add(ui);
}

Evas_Object *_products_alpha_add(const Bks_Ui *ui)
{
   return _products_list_add(ui);
}

Evas_Object *products_page_add(const Bks_Ui *ui)
{
   Evas_Object *panes;

   if (!ui || !ui->win) return;

   ui->products.panes = elm_panes_add(ui->win);
   elm_panes_horizontal_set(ui->products.panes, EINA_TRUE);

   ui.products.favs = _products_favs_add(ui);
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui->products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add();
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui->products.panes, "right", ui.products.alpha);

   return ui->products.panes;
}

void _products_page_favs_fill(const Evas_Object *grid, const *Bks_Model *model)
{
   _products_grid_fill(grid, model);
}

void _products_grid_fill(const Evas_Object *grid, const *Bks_Model *model)
void _products_page_fill(const Evas_Object *products_page, const Bks_Model *model)
{
}
