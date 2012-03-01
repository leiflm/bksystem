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


// All Data
static void _bks_model_data_get_cb(void *data, Ecore_Thread *th) {

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
   mdl.products = _bks_model_sql_products_get();
   mdl.favorite_products = _bks_model_sql_favorite_products_get(5);
   mdl.user_accounts = _bks_model_sql_user_accounts_get();
   mdl.recent_user_accounts = _bks_model_sql_recent_user_accounts_get(5);
   
   eina_lock_release(&mdl.lock);

}

static void _bks_model_data_get_finished_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_reload_finished_cb();
}

static void _bks_model_data_get_canceled_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_reload_finished_cb();
}

Ecore_Thread *_bks_model_data_get(void) {
   return ecore_thread_run(_bks_model_data_get_cb, _bks_model_data_get_finished_cb, _bks_model_data_get_canceled_cb, NULL);
}


// Sales
static void _bks_model_commit_sale_cb(void *data, Ecore_Thread *th) {
   Bks_Model_Sale *sale = (Bks_Model_Sale *)data;
   printf("Commiting sale with uid:%llu...\n", sale->uid);
   if (_bks_model_sql_commit_sale(sale)) {
      fprintf(stderr, "Failed to commit sale\n");
      sale->status = BKS_MODEL_SALE_UNFINISHED;
      ecore_thread_cancel(th);
      return;
   }
   sale->status = BKS_MODEL_SALE_DONE;

}

static void _bks_model_sale_finished_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_commit_sale_finished_cb((Bks_Model_Sale *)data);
}

static void _bks_model_sale_canceled_cb(void *data, Ecore_Thread *th) {
   printf("Sale canceld\n");
   bks_controller_model_commit_sale_finished_cb(data);
}

Ecore_Thread *_bks_model_commit_sale(const Bks_Model_Sale *sale) {
   return ecore_thread_run(_bks_model_commit_sale_cb, _bks_model_sale_finished_cb, _bks_model_sale_canceled_cb, (void*)sale);
}

// Products
static void _bks_model_products_get_cb(void *data, Ecore_Thread *th) {

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
   /*if (!products) {
      ecore_thread_cancel(th);
      eina_lock_release(&mdl.lock);
      return;
   }*/
   mdl.products = _bks_model_sql_products_get();
   mdl.favorite_products = _bks_model_sql_favorite_products_get(5);
   eina_lock_release(&mdl.lock);
}

static void _bks_model_products_get_finished_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_products_reload_finished_cb();
}

static void _bks_model_products_get_canceled_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_products_reload_finished_cb();
}

Ecore_Thread *_bks_model_products_get(const unsigned int limit) {

   return ecore_thread_run(_bks_model_products_get_cb, _bks_model_products_get_finished_cb, _bks_model_products_get_canceled_cb, &limit);

}

// User Accounts
static void _bks_model_user_accounts_get_cb(void *data, Ecore_Thread *th) {

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

   /*if (!user_accounts) {
      ecore_thread_cancel(th);
      eina_lock_release(&mdl.lock);
      return;
   }*/
   mdl.user_accounts = _bks_model_sql_user_accounts_get();
   mdl.recent_user_accounts = _bks_model_sql_recent_user_accounts_get(5);
   
   eina_lock_release(&mdl.lock);
}

static void _bks_model_user_accounts_get_finished_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_user_accounts_reload_cb();
}

static void _bks_model_user_accounts_get_canceled_cb(void *data, Ecore_Thread *th) {

   bks_controller_model_user_accounts_reload_cb();
}

Ecore_Thread *_bks_model_user_accounts_get(unsigned int limit) {

   return ecore_thread_run(_bks_model_user_accounts_get_cb, _bks_model_user_accounts_get_finished_cb, _bks_model_user_accounts_get_canceled_cb, &limit);

}

// Bill Creating
static void _bks_model_create_bill_table_cb(void *data, Ecore_Thread *th) {

   eina_lock_take(&mdl.lock);
   _bks_model_sql_create_bill_table();
   _bks_model_data_get();
   eina_lock_release(&mdl.lock);
}

static void _bks_model_create_bill_table_finished_cb(void *data, Ecore_Thread *th) {
   bks_controller_model_create_bill_table_cb();
}

static void _bks_model_create_bill_table_canceled_cb(void *data, Ecore_Thread *th) {
   bks_controller_model_create_bill_table_cb();
}

Ecore_Thread *_bks_model_create_bill_table() {
   return ecore_thread_run(_bks_model_create_bill_table_cb, _bks_model_create_bill_table_finished_cb, _bks_model_create_bill_table_canceled_cb, NULL);
}
