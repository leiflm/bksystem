#ifndef __BKS_UI_H__
#define __BKS_UI_H__

#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Types.h"
#include "Bks_Status.h"
#include "Bks_Job.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_User_Account.h"

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
        Bks_Ui_Naviframe_Page enp;
        Evas_Object *favs, *alpha, *list, *grid, *selected;
        Bks_Ui_Lockwindow lock_window;
   } products;
   struct {
        Bks_Ui_Naviframe_Page enp;
        Evas_Object *list, *index;
        Bks_Ui_Lockwindow lock_window;
   } user_accounts;
   struct {
      Evas_Object *note;
   } notification;
};

/**
 * @brief initializes the central ui.
 *
 */
void bks_ui_init(int argc, char *argv[]);

/**
 * @brief shuts the central Bks_Ui down
 *
 */
void bks_ui_shutdown(void);

/**
 * @brief Clears the ui elements (lists, etc.). Selected elements and accounts
 * are reset as well.
 */
void bks_ui_controller_clear(void);

/**
 * @brief Updates the UI to reflect the given status
 * @param status status to be reflected
 */
void bks_ui_controller_status_set(const Bks_Status status);

/**
 * @brief called, when a product sale was successful registered by the model.
 * @desc The function is called by a finished sale worker.
 * The given sale has to be freed afterwards.
 * @param sale The sale that was finished and has to be freed.
 */
void bks_ui_controller_sale_finished(Bks_Model_Sale *sale);

//User accounts page

/**
 * @brief Clears the user accounts ui elements.
 */
void bks_ui_controller_user_accounts_clear(void);

/**
 * @brief fills the UI using @p user_accounts.
 * @param accounts The user accounts used to fill the list.
 */
void bks_ui_controller_user_accounts_page_set(Eina_List *accounts);

/**
 * @brief Gets the list of selected user accounts
 * @return List of Bks_Model_User_Account elements
 */
Eina_List *bks_ui_controller_user_accounts_selected_get(void);

//Products page

/**
 * @brief Clears the products alphabeticaly sorted ui elements.
 */
void bks_ui_controller_products_alpha_clear(void);

/**
 * @brief Clears the products favourites' ui elements.
 */
void bks_ui_controller_products_favs_clear(void);

/**
 * @brief fills the UI's favourite's region using @p products.
 * @param element The thread, that has to be waited for.
 *
 */
void bks_ui_controller_products_page_favs_set(Eina_List *products);

/**
 * @brief fills the UI's alphabetic products list using @p products.
 * @param element The thread, that has to be waited for.
 */
void bks_ui_controller_products_page_alpha_set(Eina_List *products);

/**
 * @brief Gets the selected products
 * @return List of selected Bks_Model_Products.
 */
Eina_List *bks_ui_controller_products_selected_get(void);

/**
 * @brief Opens a "Open file" dialog to retrieve the location of the database
 * file.
 */
void bks_ui_controller_db_path_get(void);

#endif
