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
products_page_reset(void)
{
   _products_list_reset();
   _products_grid_reset();
}

Evas_Object *_products_favs_add(void)
{
   return bks_ui_products_grid_add();
}

Evas_Object *_products_alpha_add(void)
{
   return bks_ui_products_list_add();
}

void products_page_add(void)
{
   Evas_Object *panes;

   if (!ui || !ui->win) return;

   ui->products.panes = elm_panes_add(ui->win);
   elm_ui->products.panes_horizontal_set(ui->products.panes, EINA_TRUE);

   ui.products.favs = _products_favs_add();
   evas_object_show(ui.products.favs);
   elm_object_part_content_set(ui->products.panes, "left", ui.products.favs);

   ui.products.alpha = _products_alpha_add();
   evas_object_show(ui.products.alpha);
   elm_object_part_content_set(ui->products.panes, "right", ui.products.alpha);

   ui->products.enp.content = ui->products.panes;
   ui->products.enp.eoi = elm_naviframe_item_push(ui->naviframe, "Produkte", NULL, NULL, ui->products.enp.content, NULL);
}
