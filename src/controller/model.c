#include <Ecore.h>
#include "Bks_Ui.h"
#include "Bks_Controller.h"

static void _products_reload_cb(void *data, Ecore_Thread *th)
{
   //lock ui from further user input
   bks_ui_products_update_set(EINA_TRUE);
   //clear ui
   bks_ui_products_clear();
}

static void _user_accounts_reload_cb(void *data, Ecore_Thread *th)
{
   //lock ui from further user input
   bks_ui_user_accounts_update_set(EINA_TRUE);
   //clear ui
   bks_ui_user_accounts_clear();
}

//API callbacks for model

void bks_controller_model_user_accounts_reload_cb(void)
{
   bks_ui_user_accounts_lock_take();
   ecore_thread_run(_user_accounts_reload_cb, NULL, NULL, NULL);
}

void bks_controller_model_products_reload_cb(void)
{
   bks_ui_products_lock_take();
   ecore_thread_run(_products_reload_cb, NULL, NULL, NULL);
}

void bks_controller_model_reload_cb(void)
{
   bks_controller_model_user_accounts_reload_cb();
   bks_controller_model_products_reload_cb();
}

void bks_controller_model_user_accounts_reload_finished_cb(void)
{
   bks_ui_user_accounts_page_set(bks_model_user_accounts_get(0));
}

void bks_controller_model_products_reload_finished_cb(void)
{
   bks_ui_products_page_set(bks_model_products_get(0));
}

void bks_controller_model_reload_finished_cb(void)
{
   bks_controller_model_user_accounts_reload_finished_cb();
   bks_controller_model_products_reload_finished_cb();
}

void bks_controller_model_commit_sale_finished_cb(Bks_Model_Sale *sale)
{
        printf("Model says, sale %p is finished.\n", sale);
}

void bks_controller_model_create_bill_table_cb(void)
{
        printf("Bill created.\n");
}
