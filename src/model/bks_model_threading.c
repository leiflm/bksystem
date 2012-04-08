//      bks_model_sql.c
//
//      Copyright 2011 Matthias Wauer <matthiaswauer@googlemail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <stdlib.h>

#include <Ecore.h>
#include <Eina.h>

#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Model.h"
#include "Bks_Model_Sql.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_Threading.h"
#include "Bks_Error.h"

// error 
static void _error_ptr_free_cb(void *data) {
   free((Bks_Error *)data);
}

// All Data
static void _bks_model_data_get_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Bks_Model_User_Account *user_data;
   Bks_Model_Product *product_data;

   // tell controller that now reloading and lock model
   bks_controller_model_reload_cb();
   eina_lock_take(&mdl.lock);
   // freeing old data
   if (!mdl.user_accounts) {
      EINA_LIST_FREE(mdl.user_accounts, user_data) {
         bks_model_user_account_free(user_data);
      }
   }
   if (!mdl.recent_user_accounts) {
      EINA_LIST_FREE(mdl.recent_user_accounts, user_data) {
         bks_model_user_account_free(user_data);
      }
   }
   if (!mdl.products) {
      EINA_LIST_FREE(mdl.products, product_data) {
         bks_model_product_free(product_data);
      }
   }
   if (!mdl.favorite_products) {
      EINA_LIST_FREE(mdl.favorite_products, product_data) {
         bks_model_product_free(product_data);
      }
   }
   // getting new data
   _bks_model_sql_user_accounts_get(mdl.user_accounts);
   _bks_model_sql_recent_user_accounts_get(mdl.recent_user_accounts,10);
   _bks_model_sql_products_get(mdl.products);
   _bks_model_sql_favorite_products_get(mdl.favorite_products,10);
   eina_lock_release(&mdl.lock);

}

static void _bks_model_data_get_finished_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   printf("Loading data finished...\n");
   bks_controller_model_reload_finished_cb();
}

static void _bks_model_data_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   fprintf(stderr, "Loading data got canceled\n");
   bks_controller_model_reload_finished_cb();
}

static void _retry_ptr_free_cb(void *data) {
   int *ptr = (int*)data;
   free(ptr);
}

Ecore_Thread *_bks_model_data_get(void) {
   Ecore_Thread *th;
   th = ecore_thread_run(_bks_model_data_get_cb, _bks_model_data_get_finished_cb, _bks_model_data_get_canceled_cb, NULL);
   return th;
}



// Sales
static void _bks_model_commit_sale_cb(void *data, Ecore_Thread *th) {

   Bks_Model_Sale *sale = (Bks_Model_Sale *)data;
   int *retry_ptr;

   retry_ptr = ecore_thread_local_data_find(th, "no_of_retry");
   if (!retry_ptr) {
      retry_ptr = (int*)malloc(sizeof(int));
      if (!retry_ptr) {
         fprintf(stderr,"no memory available for sale retry counter!\n");
         ecore_thread_cancel(th);
      }
      *retry_ptr = 3;
      ecore_thread_local_data_add(th, "no_of_retry", retry_ptr, _retry_ptr_free_cb, EINA_FALSE); 
   }
   while (*retry_ptr >= 0) {
      retry_ptr = ecore_thread_local_data_find(th, "no_of_retry");
      eina_lock_take(&mdl.lock);
      printf("Commiting sale with uid:%d...\n", sale->uid);
      sale->status = _bks_model_sql_commit_sale(sale);
      switch (sale->status) {
         case BKS_MODEL_SQLITE_DATABASE_LOCKED:
         case BKS_MODEL_SQLITE_OPEN_ERROR: 
            fprintf(stderr, "Failed to commit sale,retry:%d \n", 4 - (*retry_ptr));
            (*retry_ptr)--;
            ecore_thread_local_data_set(th, "no_of_retry", retry_ptr, _retry_ptr_free_cb); 
            sale->status = BKS_MODEL_SALE_UNFINISHED;
            eina_lock_release(&mdl.lock);
            ecore_thread_reschedule(th);
            break;
         case BKS_MODEL_SALE_DONE :
            eina_lock_release(&mdl.lock);
            return;
         default:
            eina_lock_release(&mdl.lock);
            return;
      }
   }
}

static void _bks_model_sale_finished_cb(void *data, Ecore_Thread *th UNUSED) {

   bks_controller_model_commit_sale_finished_cb((Bks_Model_Sale *)data);
}

static void _bks_model_sale_canceled_cb(void *data, Ecore_Thread *th UNUSED) {
   fprintf(stderr,"Sale thread canceled\n");
   bks_controller_model_commit_sale_finished_cb((Bks_Model_Sale *)data);
}

Ecore_Thread *_bks_model_commit_sale(const Bks_Model_Sale *sale) {
   return ecore_thread_run(_bks_model_commit_sale_cb, _bks_model_sale_finished_cb, _bks_model_sale_canceled_cb, (void*)sale);
}

// Products

static void _bks_model_products_get_cb(void *data UNUSED, Ecore_Thread *th) {

   //Bks_Model_Product *product_data;
   Bks_Error *error = malloc(sizeof(Bks_Error));

   bks_controller_model_products_alpha_get_cb();
   eina_lock_take(&mdl.lock);
   // free old data (does contoller?)
   //~ if (!mdl.products) {
      //~ EINA_LIST_FREE(mdl.products, product_data) {
         //~ bks_model_product_free(product_data);
      //~ }
   //~ }
   mdl.products = NULL;
   // get new data
   *error = _bks_model_sql_products_get(mdl.products);
   eina_lock_release(&mdl.lock);
   // save error value in thread context
   ecore_thread_local_data_add(th, "error", error, _error_ptr_free_cb, EINA_FALSE);
}

static void _bks_model_products_get_finished_cb(void *data UNUSED, Ecore_Thread *th) {

   Bks_Error *error;
   error = ecore_thread_local_data_find(th,"error");
   bks_controller_model_products_get_alpha_finished_cb(mdl.products, *error);
}

static void _bks_model_products_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   fprintf(stderr,"Loading products thread canceled\n");
   bks_controller_model_products_get_alpha_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
}


// Favorite products
static void _bks_model_fav_products_get_cb(void *data, Ecore_Thread *th) {

   //Bks_Model_Product *product_data;
   Bks_Error *error = malloc(sizeof(Bks_Error));
   unsigned int *limit;
   limit = (unsigned int *)data;

   bks_controller_model_products_alpha_get_cb();
   eina_lock_take(&mdl.lock);
   // get products or favorite products
   // free old data (does contoller?)
   //~ if (!mdl.favorite_products) {
      //~ EINA_LIST_FREE(mdl.favorite_products, product_data) {
         //~ bks_model_product_free(product_data);
      //~ }
   //~ }
   mdl.favorite_products = NULL;
   // get new data
   *error = _bks_model_sql_favorite_products_get(mdl.favorite_products, *limit);
   eina_lock_release(&mdl.lock);
   
   // save error value in thread context
   ecore_thread_local_data_add(th, "error", error, _error_ptr_free_cb, EINA_FALSE);
}

static void _bks_model_fav_products_get_finished_cb(void *data UNUSED, Ecore_Thread *th) {

   Bks_Error *error;
   free((unsigned int *)data);
   error = ecore_thread_local_data_find(th,"error");
   bks_controller_model_products_get_favs_finished_cb(mdl.favorite_products, *error);
}

static void _bks_model_fav_products_get_canceled_cb(void *data, Ecore_Thread *th UNUSED) {

   free((unsigned int *)data);
   fprintf(stderr,"Loading products canceled\n");
   bks_controller_model_products_get_favs_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
   
}

Ecore_Thread *_bks_model_products_get(unsigned int limit) {

   if (limit == 0) {
      return ecore_thread_run(_bks_model_products_get_cb, _bks_model_products_get_finished_cb, _bks_model_products_get_canceled_cb, NULL);
   } else {
      unsigned int *limit_ptr = malloc(sizeof(unsigned int));
      *limit_ptr = limit; 
      return ecore_thread_run(_bks_model_fav_products_get_cb, _bks_model_fav_products_get_finished_cb, _bks_model_fav_products_get_canceled_cb, limit_ptr);
   }
}


// User Accounts
static void _bks_model_user_accounts_get_cb(void *data UNUSED, Ecore_Thread *th) {

   //Bks_Model_User_Account *user_data;
   Bks_Error *error = malloc(sizeof(Bks_Error));
   
   bks_controller_model_user_accounts_get_cb();
   eina_lock_take(&mdl.lock);
   
   // freeing old data
   //~ if (!mdl.user_accounts) {
      //~ EINA_LIST_FREE(mdl.user_accounts, user_data) {
         //~ bks_model_user_account_free(user_data);
      //~ }
   //~ }
   // get new data
   *error = _bks_model_sql_user_accounts_get(mdl.user_accounts);
   eina_lock_release(&mdl.lock);

   eina_lock_release(&mdl.lock);
   // save error value in thread context
   ecore_thread_local_data_add(th, "error", error, _error_ptr_free_cb, EINA_FALSE);
}

static void _bks_model_user_accounts_get_finished_cb(void *data UNUSED, Ecore_Thread *th) {

   Bks_Error *error;
   error = ecore_thread_local_data_find(th,"error");
   bks_controller_model_user_accounts_get_finished_cb(mdl.user_accounts, *error);
}

static void _bks_model_user_accounts_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   fprintf(stderr,"Loading user accounts canceled\n");
   bks_controller_model_user_accounts_get_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
}



Ecore_Thread *_bks_model_user_accounts_get(unsigned int limit) {
   
   if (limit == 0) {     
      return ecore_thread_run(_bks_model_user_accounts_get_cb, _bks_model_user_accounts_get_finished_cb, _bks_model_user_accounts_get_canceled_cb, &limit);
   } else {
      fprintf(stderr, "Recent user accounts UNUSED\n");
      return NULL;
   }
}

