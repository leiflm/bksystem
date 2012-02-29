#ifndef __BKS_UI_PRIVATE_H__
#define __BKS_UI_PRIVATE_H__

#include "Bks_Types.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"

Evas_Object *bks_ui_win_new(void);

// user acounts page related functions
Evas_Object *user_accounts_page_add(void);
Evas_Object *user_accounts_page_fill(void);
void user_accounts_page_reset(void);
// list related:
void user_accounts_list_fill(void);

// products page related functions
Evas_Object *products_page_add(void);
void products_page_fill(void);
void products_page_reset(void);
//grid (favs) related:
Evas_Object *products_grid_add(void);
void products_grid_fill(void);
void products_grid_reset(Evas_Object *products_page);
//normal list related:
Evas_Object *products_list_add(void);
void products_list_fill(void);
void products_list_reset(Evas_Object *products_page);

void products_product_selected(Bks_Model_Product *product);

#endif
