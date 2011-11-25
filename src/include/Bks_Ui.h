#ifndef __BKS_UI_H__
#define __BKS_UI_H__

#include "Bks_Error.h"
#include "Bks_Controller.h"

typedef struct _Bks_Ui Bks_Ui;
typedef struct _Bks_Elm_Naviframe_Page Bks_Elm_Naviframe_Page;

struct _Bks_Elm_Naviframe_Page {
      Evas_Object *content, *prev_btn, *next_btn;
      Elm_Object_Item *eoi;
};

struct _Bks_Ui {
   Evas_Object *win, *naviframe;
   struct {
        Bks_Elm_Naviframe_Page enp;
        Evas_Object *panes, *favs, *alpha, *list, *grid;
   } products;
   struct {
        Bks_Elm_Naviframe_Page enp;
        Evas_Object *list;
   } user_accounts;
};

/**
 * @brief creates and initializes a new Bks_Ui item
 *
 * @param controller The controller event callbacks will be registered to
 * @param model The model the data is fetched from and written to
 *
 * @return Bks_Ui instance to be used for callbacks in controller
 */
Bks_Ui *bks_ui_new(const Bks_Controller *controller, const Bks_Model *model);

/**
 * @param ui The ui that shall be shut down.
 *
 */
void bks_ui_shutdown(void);

/**
 * @param ui The Bks_Ui the selected user accounts are retrieved from
 * @return List of Bks_User_Account elements
 */
Eina_List *bks_ui_user_accounts_selected_get(void);
const Bks_Product *bks_ui_product_selected_get(void);

/**
 * @param ui The Bks_Ui the selected user accounts are retrieved from
 * @param data Bks_Error
 */
void bks_ui_display_error(const Bks_Error error);

/**
 * @brief called, when a product sale was successful.
 */
void bks_ui_sale_finished(void);

/**
 * @brief called, when a product sale failed.
 */
void bks_ui_sale_failed(const Bks_Error error);


/**
 * @param thread Instance that failed
 */
//void bks_ui_model_data_fetch_cancel(void *data, Ecore_Thread *thread);

/**
 * @brief called, when data is successfully fetched
 * @desc callback called by the model, when data was sucessfully fetched
 * into memory globally shared using the key "bks.model.users"
 * @param data Bks_Error
 * @param thread Instance that failed
 */
//void bks_ui_model_data_fetch_finish(void *data, Ecore_Thread *thread);
#endif
