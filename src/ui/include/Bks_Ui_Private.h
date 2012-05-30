#ifndef __BKS_UI_PRIVATE_H__
#define __BKS_UI_PRIVATE_H__

#include "Bks_Types.h"
#include "Bks_Ui.h"
#include "Bks_Model_Product.h"

//Convenience Macros
#define EXPAND(o) evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
#define FILL(o) evas_object_size_hint_fill_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
#define EXPAND_AND_FILL(o) EXPAND(o);\
                           FILL(o)

Evas_Object *bks_ui_win_add(void);

// user acounts page related functions
Evas_Object *user_accounts_page_add(void);
void user_accounts_page_set(Eina_List *user_accounts);
void user_accounts_page_clear(void);
// unselects user accounts
void user_accounts_page_reset(void);
//normal list related:
Evas_Object *user_accounts_page_list_add();

// list related:
void user_accounts_list_set(Eina_List *user_accounts);

// products page related functions
Evas_Object *products_page_add(void);
void products_page_clear();
// unselects selected product
void products_page_reset(void);
//grid (favs) related:
Evas_Object *products_grid_add(void);
void products_grid_set(Eina_List *products);
// unselects selected product
void products_grid_reset(void);
void _products_grid_clear(void);
//normal list related:
Evas_Object *products_list_add(void);
void _products_list_clear(void);
void products_list_set(Eina_List *products);
// unselects selected product
void products_list_reset(void);

void _product_selected_set(const Elm_Object_Item *product);

void _bks_ui_sale_notification_set(void *data);
void _bks_ui_sale_notification_clicked(void *data, Evas_Object *obj, void *event_info);

#endif
