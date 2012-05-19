#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Job.h"
#include "Bks_Ui.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"


void bks_controller_init(void)
{
}

void bks_controller_shutdown(void)
{
}

void bks_controller_run(void)
{
    bks_controller_products_alpha_get();
    bks_controller_products_favs_get(5);
    bks_controller_user_accounts_get();
    ecore_main_loop_begin();
}

void
_products_alpha_get(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   job->data = NULL;
   job->status = bks_model_controller_products_alpha_get((Eina_List**)&job->data);
}

void
_products_alpha_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
         bks_ui_controller_db_path_get(job);
      case BKS_MODEL_SQLITE_OK:
         bks_job_free(job);
         bks_ui_controller_products_page_alpha_set((Eina_List*)job->data);
         printf("DB access was successful!\n");
      default:
         printf("Some error during alpha products fetch!\n");
     }
}

void
bks_controller_products_alpha_get(void)
{
   Bks_Job *job = bks_job_new(BKS_JOB_PRODUCTS_FAVS_GET);

   ecore_thread_run(_products_alpha_get, _products_alpha_get_finished, NULL, job);
}

void
_products_favs_get(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;
   unsigned int count;

   EINA_SAFETY_ON_NULL_RETURN(job);

   count = (unsigned int)job->data;
   job->data = NULL;
   job->status = bks_model_controller_products_fav_get((Eina_List**)&job->data, count);
}

void
_products_favs_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
         bks_ui_controller_db_path_get(job);
      case BKS_MODEL_SQLITE_OK:
         bks_job_free(job);
         bks_ui_controller_products_page_favs_set((Eina_List*)job->data);
         printf("DB access was successful!\n");
      default:
         printf("Some error during favs products fetch!\n");
     }
}

void
bks_controller_products_favs_get(unsigned int count)
{
   Bks_Job *job = bks_job_new(BKS_JOB_PRODUCTS_ALPHA_GET);

   job->data = (void*)count;
   ecore_thread_run(_products_favs_get, _products_favs_get_finished, NULL, job);
}

void
_user_accounts_get(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   job->data = NULL;
   job->status = bks_model_controller_user_accounts_get((Eina_List**)job->data);
}

void
_user_accounts_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
         bks_ui_controller_db_path_get(job);
      case BKS_MODEL_SQLITE_OK:
         bks_job_free(job);
         bks_ui_controller_user_accounts_page_set((Eina_List*)job->data);
         printf("DB access was successful!\n");
      default:
         printf("Some error during user accounts fetch!\n");
     }
}

void
bks_controller_user_accounts_get(void)
{
   Bks_Job *job = bks_job_new(BKS_JOB_PRODUCTS_ALPHA_GET);

   ecore_thread_run(_user_accounts_get, _user_accounts_get_finished, NULL, job);
}

// API for UI callbacks
void
_sale(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   job->status = bks_model_controller_commit_sale((Bks_Model_Sale*)job->data);
}

void _sale_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
         bks_ui_controller_db_path_get(job);
      case BKS_MODEL_SQLITE_OK:
         bks_job_free(job);
         bks_ui_controller_sale_finished((Bks_Model_Sale*)job->data);
         printf("DB access was successful!\n");
      default:
         printf("Some error during sale!\n");
     }
}

void bks_controller_ui_sale_finish(void)
{
   const Eina_List *accounts = NULL;
   const Bks_Model_Product *product = NULL;
   Bks_Model_Sale *sale = NULL;
   Bks_Job *job = bks_job_new(BKS_JOB_SALE);

   product = bks_ui_controller_product_selected_get();
   accounts = bks_ui_controller_user_accounts_selected_get();

   EINA_SAFETY_ON_NULL_GOTO(product, _incomplete_sale);
   EINA_SAFETY_ON_NULL_GOTO(accounts, _incomplete_sale);

   sale = bks_model_sale_new(accounts, product);

   if (!sale)
     {
        printf("Creating sale failed, skipping!\n");
     }

   job->data = sale;

   //CREATE ECORE_THREAD that calls ui cb when finished
   ecore_thread_run(_sale, _sale_finished, NULL, job);

   return;

_incomplete_sale:
   printf("UI sale finished, but product is:%p and customer: %p.\n", product, accounts);
}

void _job_continue(Bks_Job *job)
{
   EINA_SAFETY_ON_NULL_RETURN(job);

   switch(job->type)
     {
      case BKS_JOB_PRODUCTS_ALPHA_GET:
         bks_controller_products_alpha_get();
         break;
      case BKS_JOB_PRODUCTS_FAVS_GET:
         bks_controller_products_favs_get((unsigned int)job->data);
         break;
      case BKS_JOB_SALE:
         bks_model_controller_commit_sale((Bks_Model_Sale*)job->data);
         break;
      default:
         printf("Unkown job type (%d) scheduled for continuation.\n", job->type);
         break;
     }
}

void bks_controller_ui_db_path_retrieved(Bks_Job *job)
{
   EINA_SAFETY_ON_NULL_RETURN(job);

   bks_model_controller_db_path_set((Eina_Stringshare*)job->data);
   _job_continue(job);
}
