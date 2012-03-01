#ifndef __BKS_UI_H__
#define __BKS_UI_H__

#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Error.h"
#include "Bks_Types.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"

typedef struct _Bks_Elm_Naviframe_Page Bks_Elm_Naviframe_Page;

struct _Bks_Elm_Naviframe_Page {
      Evas_Object *content, *prev_btn, *next_btn;
      Elm_Object_Item *eoi;
};

struct _Bks_Ui {
   Evas_Object *win, *naviframe;
   struct {
        Eina_Lock lock;
        Bks_Elm_Naviframe_Page enp;
        Evas_Object *panes, *favs, *alpha, *list, *grid;
   } products;
   struct {
        Eina_Lock lock;
        Bks_Elm_Naviframe_Page enp;
        Evas_Object *list;
   } user_accounts;
};

/**
 * @brief initializes the central ui.
 *
 */
void bks_ui_init(void);

/**
 * @brief shuts the central Bks_Ui down
 *
 */
void bks_ui_shutdown(void);

/**
 * @brief Clears the ui elements (lists, etc.). Selected elements and accounts
 * are reset as well.
 */
void bks_ui_clear(void);

/**
 * @brief Locks the ui so no input is possible anymore.
 */
void bks_ui_lockscreen_visible_set(const Eina_Bool visible);

/**
 * @brief generic interface to display an error that occured.
 * @param data Bks_Error
 */
void bks_ui_display_error(const Bks_Error error);

/**
 * @brief called, when a product sale was successful registered by the model.
 * @desc The function is called by a finished sale worker.
 * The given sale has to be freed afterwards.
 * @param sale The sale that was finished and has to be freed.
 */
void bks_ui_model_sale_finished_cb(Bks_Model_Sale *sale);

//User accounts page

/**
 * @brief Clears the user accounts ui elements.
 */
void bks_ui_user_accounts_clear(void);

/**
 * @brief Indicates that the user accounts are being refetched.
 */
void bks_ui_user_accounts_update_set(const Eina_Bool update);

/**
 * @brief Aquires the lock for the user accounts ui elements
 */
void bks_ui_user_accounts_lock_take(void);

/**
 * @brief fills the UI using @p user_accounts.
 *
 * @param user_accounts list of Bks_ModeL_User_Account.
 */
void bks_ui_user_accounts_page_set(const Eina_List *user_accounts);

/**
 * @brief Gets the list of selected user accounts
 * @return List of Bks_Model_User_Account elements
 */
Eina_List *bks_ui_user_accounts_selected_get(void);

//Products page

/**
 * @brief Clears the products ui elements.
 */
void bks_ui_products_clear(void);

/**
 * @brief Indicates that the products are being refetched.
 */
void bks_ui_products_update_set(const Eina_Bool update);

/**
 * @brief Aquires a lock for the products ui elements
 */
void bks_ui_products_lock_take(void);

/**
 * @brief fills the UI using @p products.
 *
 * @param products list of Bks_ModeL_Product
 */
void bks_ui_products_page_set(const Eina_List *products);

/**
 * @brief Gets the selected product
 * @return The selected Bks_Model_Product element.
 */
const Bks_Model_Product *bks_ui_product_selected_get(void);
#endif
