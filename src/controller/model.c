#include <Ecore.h>
#include "Bks_Types.h"
#include "Bks_Ui.h"
#include "Bks_Controller.h"
#include "Bks_Thread_Queue.h"

//API callbacks for model

void bks_controller_model_user_accounts_reload_cb(void)
{
   Bks_Thread_Queue_Element *tqe = NULL;

   tqe = bks_thread_queue_append(BKS_THREAD_TYPE_USER_ACCOUNTS_GET, ecore_time_get());
   EINA_SAFETY_ON_NULL_RETURN(tqe);
   printf("Job zum leeren der Benutzerlisten hinzugefügt (%p).\n", tqe);

   //clear ui
   bks_ui_user_accounts_clear(tqe);
}

void bks_controller_model_products_reload_cb(void)
{
   Bks_Thread_Queue_Element *tqe = NULL;

   tqe = bks_thread_queue_append(BKS_THREAD_TYPE_PRODUCTS_GET, ecore_time_get());
   EINA_SAFETY_ON_NULL_RETURN(tqe);
   printf("Job zum leeren der Produktlisten hinzugefügt (%p).\n", tqe);

   //clear ui
   bks_ui_products_clear(tqe);
}

void bks_controller_model_reload_cb(void)
{
   bks_controller_model_user_accounts_reload_cb();
   bks_controller_model_products_reload_cb();
}

void bks_controller_model_user_accounts_reload_finished_cb(void)
{
   Eina_List *user_accounts = NULL;
   Bks_Thread_Queue_Element *tqe = NULL;

   tqe = bks_thread_queue_append(BKS_THREAD_TYPE_USER_ACCOUNTS_GET_FINISHED, ecore_time_get());
   EINA_SAFETY_ON_NULL_RETURN(tqe);
   printf("Job zum fuellen der Benutzerlisten hinzugefügt (%p).\n", tqe);
   user_accounts = bks_model_user_accounts_get(0);
   tqe->data = (void*)user_accounts;
   bks_ui_user_accounts_page_set(tqe);
}

void bks_controller_model_products_reload_finished_cb(void)
{
   Eina_List *products = NULL;
   Bks_Thread_Queue_Element *tqe = NULL;

   tqe = bks_thread_queue_append(BKS_THREAD_TYPE_PRODUCTS_GET_FINISHED, ecore_time_get());
   EINA_SAFETY_ON_NULL_RETURN(tqe);
   printf("Job zum fuellen der alphabetischen Produktliste hinzugefügt (%p).\n", tqe);
   products = bks_model_products_get(0);
   tqe->data = (void*)products;
   bks_ui_products_page_alpha_set(tqe);

   tqe = bks_thread_queue_append(BKS_THREAD_TYPE_PRODUCTS_GET_FINISHED, ecore_time_get());
   EINA_SAFETY_ON_NULL_RETURN(tqe);
   printf("Job zum fuellen der Favoritenproduktliste hinzugefügt (%p).\n", tqe);
   products = bks_model_products_get(6);
   tqe->data = (void*)products;
   bks_ui_products_page_favs_set(tqe);
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
