#include <sys/stat.h>
#include <fcntl.h>
#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Job.h"
#include "Bks_Ui.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"

#define LOCK_FILE_PATH "/tmp/bksystem"

/*
 * General description of event sequence:
 * 1.) UI or Model trigger event
 * 2.) The controller creates a thread and attaches a Bks_Job that contains:
 *      - The Type
 *      - Its status (e.g. error)
 *      - A pointer to optional data
 * 3.) When the thread finished, a callback is called that checks the Bks_Job's
 *     status and triggers e.g. an UI update or the commit of a sale.
 * 4.) Free the Bks_Job
 */

Eina_Bool _event_refresh_data(void *data UNUSED)
{
   int f = -1;

   f = open(LOCK_FILE_PATH, (O_RDWR | O_CREAT | O_EXCL));

   /*
    * Single instance approach:
    * 1. Try to take lock
    *   on success: continue normally
    *   on failure and user wants to open anyway:
    *       Try to find window of the other instance
    *       1. If found, raise it
    *       2. If not found, carry on normally, lock file is a left over
    */
   if (f < 0)
   {
       bks_ui_controller_singleton_display();
       ctrl.is_primary_instance = EINA_FALSE;
   }
   else
   {
       close(f);
       bks_ui_controller_main_show();
   }

   bks_controller_products_alpha_get();
   bks_controller_products_favs_get(5);
   bks_controller_user_accounts_favs_get(25);

   return ECORE_CALLBACK_CANCEL;
}

void bks_controller_init(void)
{
   eina_lock_new(&ctrl.db_lock);
   ctrl.is_primary_instance = EINA_TRUE;
}

void bks_controller_shutdown(void)
{
   eina_lock_free(&ctrl.db_lock);
   if (ctrl.is_primary_instance)
       ecore_file_remove(LOCK_FILE_PATH);
}

void bks_controller_run(void)
{
   //Add events processed later by the main loop
   ecore_timer_add(0.01, _event_refresh_data, NULL);
   ecore_main_loop_begin();
}

void
_products_alpha_get(void *data, Ecore_Thread *th)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   job->data = NULL;
   if (!eina_lock_take_try(&ctrl.db_lock)) {
      job->status = BKS_MODEL_SQLITE_OPEN_RUNNING;

   } else {
      eina_lock_release(&ctrl.db_lock);
      job->status = bks_model_controller_products_alpha_get((Eina_List**)&job->data);
   }

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
        if (eina_lock_take_try(&ctrl.db_lock))
          bks_ui_controller_db_path_get();
      case BKS_MODEL_SQLITE_OPEN_RUNNING:
        ecore_thread_reschedule(th);
        break;
      default:
        break;
     }

}

void
_products_alpha_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   /*
   if (eina_lock_take_try(&ctrl.db_lock))
     ;
     */
   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OK:
         //bks_ui_controller_products_alpha_clear();
         bks_ui_controller_products_page_alpha_set((Eina_List*)job->data);
         bks_job_free(job);
#ifdef DEBUG
         printf("DB access was successful!\n");
#endif
         break;
      default:
         printf("%s error during alpha products fetch!\n", BKS_STATUS_STRINGS[job->status]);
         break;
     }
}

void
bks_controller_products_alpha_get(void)
{
   Bks_Job *job = bks_job_new(BKS_JOB_PRODUCTS_ALPHA_GET);

   ecore_thread_run(_products_alpha_get, _products_alpha_get_finished, NULL, job);
}

void
_products_favs_get(void *data, Ecore_Thread *th)
{
   Bks_Job *job = (Bks_Job*)data;
   unsigned int count;

   EINA_SAFETY_ON_NULL_RETURN(job);
   count = (unsigned int)job->data;
   if (!eina_lock_take_try(&ctrl.db_lock)) {
      job->status = BKS_MODEL_SQLITE_OPEN_RUNNING;
   } else {
      eina_lock_release(&ctrl.db_lock);
      job->data = NULL;
      job->status = bks_model_controller_products_fav_get((Eina_List**)&job->data, count);
   }
   
   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
        if (eina_lock_take_try(&ctrl.db_lock))
          bks_ui_controller_db_path_get();
      case BKS_MODEL_SQLITE_OPEN_RUNNING:
        job->data = (void*)count;
        ecore_thread_reschedule(th);
        break;
      default:
        break;
     }

}

void
_products_favs_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OK:
         //bks_ui_controller_products_favs_clear();
         bks_ui_controller_products_page_favs_set((Eina_List*)job->data);
         bks_job_free(job);
#ifdef DEBUG
         printf("DB access was successful!\n");
#endif
         break;
      default:
         printf("%s error during alpha products fetch!\n", BKS_STATUS_STRINGS[job->status]);
         break;
     }

}

void
bks_controller_products_favs_get(unsigned int count)
{
   Bks_Job *job = bks_job_new(BKS_JOB_PRODUCTS_FAVS_GET);

   job->data = (void*)count;
   ecore_thread_run(_products_favs_get, _products_favs_get_finished, NULL, job);
}

void
_user_accounts_favs_get(void *data, Ecore_Thread *th)
{
   Bks_Job *job = (Bks_Job*)data;
   unsigned int limit;
   
   limit = (unsigned int)job->data;
   if (!eina_lock_take_try(&ctrl.db_lock)) {
      job->status = BKS_MODEL_SQLITE_OPEN_RUNNING;
   } else {
      eina_lock_release(&ctrl.db_lock);
      job->data = NULL;
      job->status = bks_model_controller_user_accounts_fav_get((Eina_List**)&job->data, limit);
   }

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
        if (eina_lock_take_try(&ctrl.db_lock))
          bks_ui_controller_db_path_get();
      case BKS_MODEL_SQLITE_OPEN_RUNNING:
        ecore_thread_reschedule(th);
        break;
      default:
        break;
     }

}

void
_user_accounts_favs_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OK:
         //bks_ui_controller_user_accounts_clear();
         bks_ui_controller_user_accounts_page_set((Eina_List*)job->data);
         bks_job_free(job);
#ifdef DEBUG
         printf("DB access was successful!\n");
#endif
         break;
      default:
         printf("%s error during user accounts favs fetch!\n", BKS_STATUS_STRINGS[job->status]);
     }
}

void
bks_controller_user_accounts_favs_get(unsigned int limit)
{
   Bks_Job *job = bks_job_new(BKS_JOB_USER_ACCOUNTS_FAVS_GET);

   job->data = (void*)limit;
   ecore_thread_run(_user_accounts_favs_get, _user_accounts_favs_get_finished, NULL, job);
}

void
_user_accounts_alpha_get(void *data, Ecore_Thread *th)
{
   Bks_Job *job = (Bks_Job*)data;
   
   if (!eina_lock_take_try(&ctrl.db_lock)) {
      job->status = BKS_MODEL_SQLITE_OPEN_RUNNING;
   } else {
      eina_lock_release(&ctrl.db_lock);
      job->data = NULL;
      job->status = bks_model_controller_user_accounts_alpha_get((Eina_List**)&job->data);
   }

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
        if (eina_lock_take_try(&ctrl.db_lock))
          bks_ui_controller_db_path_get();
      case BKS_MODEL_SQLITE_OPEN_RUNNING:
        ecore_thread_reschedule(th);
        break;
      default:
        break;
     }

}

void
_user_accounts_alpha_get_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;

   EINA_SAFETY_ON_NULL_RETURN(job);

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OK:
         //bks_ui_controller_user_accounts_clear();
         bks_ui_controller_user_accounts_page_set((Eina_List*)job->data);
         bks_job_free(job);
#ifdef DEBUG
         printf("DB access was successful!\n");
#endif
         break;
      default:
         printf("%s error during alpha products fetch!\n", BKS_STATUS_STRINGS[job->status]);
     }
}

void
bks_controller_user_accounts_alpha_get(void)
{
   Bks_Job *job = bks_job_new(BKS_JOB_USER_ACCOUNTS_ALPHA_GET);

   ecore_thread_run(_user_accounts_alpha_get, _user_accounts_alpha_get_finished, NULL, job);
}

// API for UI callbacks
void
_sale(void *data, Ecore_Thread *th)
{
   Bks_Job *job = (Bks_Job*)data;
   if (!eina_lock_take_try(&ctrl.db_lock)) {
      job->status = BKS_MODEL_SQLITE_OPEN_RUNNING;

   } else {
      eina_lock_release(&ctrl.db_lock);
      job->status = bks_model_controller_commit_sale((Bks_Model_Sale*)job->data);
   }

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OPEN_ERROR:
        if (eina_lock_take_try(&ctrl.db_lock))
          bks_ui_controller_db_path_get();
      case BKS_MODEL_SQLITE_OPEN_RUNNING:
        ecore_thread_reschedule(th);
        break;
      default:
        break;
     }

}

void _sale_finished(void *data, Ecore_Thread *th UNUSED)
{
   Bks_Job *job = (Bks_Job*)data;
   Bks_Model_Sale *sale;

   EINA_SAFETY_ON_NULL_RETURN(job);
   sale = (Bks_Model_Sale*)job->data;

   switch (job->status)
     {
      case BKS_MODEL_SQLITE_OK:
         sale->status = BKS_MODEL_SALE_DONE;
#ifdef DEBUG
         printf("DB access was successful!\n");
#endif
         break;
      default:
         sale->status = BKS_MODEL_SALE_ERROR;
         printf("Some error during sale!\n");
         break;
     }
   bks_ui_controller_sale_finished(sale);
   bks_job_free(job);

}

void bks_controller_ui_sale_finish(void)
{
   Eina_List *accounts = NULL, *products;
   Bks_Model_Sale *sale = NULL;
   Bks_Job *job = bks_job_new(BKS_JOB_SALE);

   products = bks_ui_controller_products_selected_get();
   accounts = bks_ui_controller_user_accounts_selected_get();

   EINA_SAFETY_ON_NULL_GOTO(products, _incomplete_sale);
   EINA_SAFETY_ON_NULL_GOTO(accounts, _incomplete_sale);

   sale = bks_model_sale_new(accounts, products);

   if (!sale)
     {
        printf("Creating sale failed, skipping!\n");
     }

   job->data = sale;

   //CREATE ECORE_THREAD that calls ui cb when finished
   ecore_thread_run(_sale, _sale_finished, NULL, job);

   return;

_incomplete_sale:
   printf("UI sale finished, but products is:%p and customer: %p.\n", products, accounts);
}

void bks_controller_ui_db_path_retrieved(Eina_Stringshare *path)
{
   bks_model_controller_db_path_set(path);
   eina_lock_release(&ctrl.db_lock);
}

void bks_controller_ui_start_even_if_lock_exists(void)
{
    // If another instance is found running, raise it and exit this instance
    if (bks_ui_controller_singleton_raised())
        ecore_main_loop_quit();
    ctrl.is_primary_instance = EINA_TRUE;
    bks_ui_controller_main_show();
}
