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
   mdl.user_accounts = _bks_model_sql_user_accounts_get();
   mdl.recent_user_accounts = _bks_model_sql_recent_user_accounts_get(10);
   mdl.products = _bks_model_sql_products_get();
   mdl.favorite_products = _bks_model_sql_favorite_products_get(10);
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
      printf("Commiting sale with uid:%llu...\n", sale->uid);
      sale->status = _bks_model_sql_commit_sale(sale);
      if (sale->status == BKS_MODEL_SQLITE_DATABASE_LOCKED) {
         fprintf(stderr, "Failed to commit sale,retry:%d \n", *retry_ptr);
         (*retry_ptr)--;
         ecore_thread_local_data_set(th, "no_of_retry", retry_ptr, _retry_ptr_free_cb); 
         sale->status = BKS_MODEL_SALE_UNFINISHED;
         eina_lock_release(&mdl.lock);
         ecore_thread_reschedule(th);
      } else {
         eina_lock_release(&mdl.lock);
         break;
      }
   }
}

static void _bks_model_sale_finished_cb(void *data, Ecore_Thread *th UNUSED) {

   bks_controller_model_commit_sale_finished_cb((Bks_Model_Sale *)data);
}

static void _bks_model_sale_canceled_cb(void *data, Ecore_Thread *th UNUSED) {
   fprintf(stderr,"Sale canceled\n");
   bks_controller_model_commit_sale_finished_cb(data);
}

Ecore_Thread *_bks_model_commit_sale(const Bks_Model_Sale *sale) {
   return ecore_thread_run(_bks_model_commit_sale_cb, _bks_model_sale_finished_cb, _bks_model_sale_canceled_cb, (void*)sale);
}

// Products
static void _bks_model_products_get_cb(void *data UNUSED, Ecore_Thread *th) {

   Bks_Model_Product *product_data;

   bks_controller_model_products_reload_cb();
   eina_lock_take(&mdl.lock);

   // free old data
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

   mdl.products = _bks_model_sql_products_get();
   if (!mdl.products) {
      eina_lock_release(&mdl.lock);
      ecore_thread_cancel(th);
      return;
   }
   mdl.favorite_products = _bks_model_sql_favorite_products_get(5);
   eina_lock_release(&mdl.lock);
}

static void _bks_model_products_get_finished_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   bks_controller_model_products_reload_finished_cb();
}

static void _bks_model_products_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   fprintf(stderr,"Loading products canceled\n");
   bks_controller_model_products_reload_finished_cb();
}

Ecore_Thread *_bks_model_products_get(const unsigned int limit) {
   //FIXME: don't pass pointer of stack variable!!!
   return ecore_thread_run(_bks_model_products_get_cb, _bks_model_products_get_finished_cb, _bks_model_products_get_canceled_cb, &limit);

}

// User Accounts
static void _bks_model_user_accounts_get_cb(void *data UNUSED, Ecore_Thread *th) {

   Bks_Model_User_Account *user_data;
   bks_controller_model_user_accounts_reload_cb();
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

   mdl.user_accounts = _bks_model_sql_user_accounts_get();
   if (!mdl.user_accounts) {
      eina_lock_release(&mdl.lock);
      ecore_thread_cancel(th);
      return;
   }
   mdl.recent_user_accounts = _bks_model_sql_recent_user_accounts_get(5);
   
   eina_lock_release(&mdl.lock);
}

static void _bks_model_user_accounts_get_finished_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   bks_controller_model_user_accounts_reload_finished_cb();
}

static void _bks_model_user_accounts_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   fprintf(stderr,"Loading user accounts canceled\n");
   bks_controller_model_user_accounts_reload_finished_cb();
}

Ecore_Thread *_bks_model_user_accounts_get(unsigned int limit) {
   //FIXME: don't pass pointer of stack variable!!!
   return ecore_thread_run(_bks_model_user_accounts_get_cb, _bks_model_user_accounts_get_finished_cb, _bks_model_user_accounts_get_canceled_cb, &limit);

}

