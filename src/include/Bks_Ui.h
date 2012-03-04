#ifndef __BKS_UI_H__
#define __BKS_UI_H__

#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Error.h"
#include "Bks_Types.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"

typedef struct _Bks_Ui_Naviframe_Page Bks_Ui_Naviframe_Page;
typedef struct _Bks_Ui_Lockwindow Bks_Ui_Lockwindow;

struct _Bks_Ui_Naviframe_Page {
      Evas_Object *content, *prev_btn, *next_btn;
      Elm_Object_Item *eoi;
};

struct _Bks_Ui_Lockwindow {
     Evas_Object *win, *content;
};

struct _Bks_Ui {
   Evas_Object *win, *naviframe;
   Bks_Ui_Lockwindow lock_window;
   struct {
        struct {
             Eina_Lock favs;
             Eina_Lock alpha;
        } locks;
        Bks_Ui_Naviframe_Page enp;
        Evas_Object *panes, *favs, *alpha, *list, *grid;
        const Elm_Object_Item *selected;
        Bks_Ui_Lockwindow lock_window;
   } products;
   struct {
        Eina_Lock lock;
        Bks_Ui_Naviframe_Page enp;
        Evas_Object *list;
        Bks_Ui_Lockwindow lock_window;
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
void bks_ui_update_set(const Eina_Bool update);

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
void bks_ui_contoller_sale_finished_cb(Bks_Model_Sale *sale);

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
 * @brief fills the UI using @p user_accounts.
 *
 * @param user_accounts list of Bks_ModeL_User_Account.
 */
void bks_ui_user_accounts_page_set(Eina_List *user_accounts);

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
 * @brief fills the UI's favourite's region using @p products.
 *
 * @param products list of Bks_ModeL_Product
 */
void bks_ui_products_page_favs_set(Eina_List *products);

/**
 * @brief fills the UI's alphabetic products list using @p products.
 *
 * @param products list of Bks_ModeL_Product
 */
void bks_ui_products_page_alpha_set(Eina_List *products);

/**
 * @brief Gets the selected product
 * @return The selected Bks_Model_Product element.
 */
const Bks_Model_Product *bks_ui_product_selected_get(void);
#endif
