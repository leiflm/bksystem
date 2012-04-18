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


static void _retry_ptr_free_cb(void *data) {
   free((int*)data);
}

// save path of DB to file
static void _bks_model_save_path_cb(void *data, Ecore_Thread *th UNUSED) {

   char *path;
   //~ Eet_File *file;
   path = (char *)data;
   fprintf(stderr, "Path: %s now saved to file! TODO!!\n", path);
   //~ eet_init();
   //~ file = eet_open("path", EET_FILE_MODE_WRITE);
   //~ eet_delete(file, "path");
   //~ eet_write(file, "path", path, strlen(path), 0);
   //~ eet_close(file);
   //~ eet_shutdown();
}

static void _bks_model_save_path_finished_cb(void *data, Ecore_Thread *th UNUSED) {
   free((char *)data);
}

static void _bks_model_save_path_canceled_cb(void *data, Ecore_Thread *th UNUSED) {
   free((char *)data);
}

Ecore_Thread *_bks_model_save_path(const char *path) {
   return ecore_thread_run(_bks_model_save_path_cb, _bks_model_save_path_finished_cb, _bks_model_save_path_canceled_cb, (void*)path);
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

static void _bks_model_products_get_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Eina_List **products = calloc(1, sizeof(Eina_List*)); 
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;
 
   bks_controller_model_products_alpha_get_cb();
   //eina_lock_take(&mdl.lock);

   // get new data
   th_data->error = _bks_model_sql_products_get(products);
   th_data->list = *products;
   //eina_lock_release(&mdl.lock);
}

static void _bks_model_products_get_finished_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;

   bks_controller_model_products_alpha_get_finished_cb(th_data->list, th_data->error);
   free(th_data);
}

static void _bks_model_products_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;
   Bks_Model_Product *current_product;

   //Since we don't pass the list, we must free it, if exists
   if (!th_data->list) { // list was created
      EINA_LIST_FREE(th_data->list, current_product) {
         bks_model_product_free(current_product);
      }
   }
   free((Bks_Thread_Data*)th_data);
   fprintf(stderr,"Loading products canceled\n");
   bks_controller_model_products_alpha_get_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
}


// Favorite products
static void _bks_model_fav_products_get_cb(void *data, Ecore_Thread *th UNUSED) {

   Eina_List **products = calloc(1, sizeof(Eina_List*)); 
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;

   bks_controller_model_products_favs_get_cb();
   //eina_locl_take(&mdl.lock);

   // get new data
   th_data->error = _bks_model_sql_favorite_products_get(products, th_data->limit);
   th_data->list = *products;
   //eina_locl_release(&mdl.lock);
   
}

static void _bks_model_fav_products_get_finished_cb(void *data, Ecore_Thread *th UNUSED) {
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;

   bks_controller_model_products_favs_get_finished_cb(th_data->list, th_data->error);
   free(th_data);
}

static void _bks_model_fav_products_get_canceled_cb(void *data, Ecore_Thread *th UNUSED) {
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;
   Bks_Model_Product *current_product;

   //Since we don't pass the list, we must free it, if exists

   if (!th_data->list) { // list was created
      EINA_LIST_FREE(th_data->list, current_product) {
         bks_model_product_free(current_product);
      }
   }
   free((Bks_Thread_Data*)th_data);
   fprintf(stderr,"Loading products canceled\n");
   bks_controller_model_products_favs_get_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
   
}

Ecore_Thread *_bks_model_products_get(unsigned int limit) {

   Bks_Thread_Data *data;
   data = calloc(1, sizeof(Bks_Thread_Data));
   data->list = NULL;
   data->limit = limit;

   if (limit == 0) {
      return ecore_thread_run(_bks_model_products_get_cb, _bks_model_products_get_finished_cb, _bks_model_products_get_canceled_cb, data);
   } else {
      return ecore_thread_run(_bks_model_fav_products_get_cb, _bks_model_fav_products_get_finished_cb, _bks_model_fav_products_get_canceled_cb, data);
   }
}


// User Accounts
static void _bks_model_user_accounts_get_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Eina_List **user_accounts = calloc(1, sizeof(Eina_List*)); 
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;
 
   bks_controller_model_user_accounts_get_cb();
   //eina_locl_take(&mdl.lock);

   // get new data
   th_data->error = _bks_model_sql_user_accounts_get(user_accounts);
   th_data->list = *user_accounts;
   //eina_locl_release(&mdl.lock);
      
}

static void _bks_model_user_accounts_get_finished_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {

   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;

   bks_controller_model_user_accounts_get_finished_cb(th_data->list, th_data->error);
   free(th_data);
}

static void _bks_model_user_accounts_get_canceled_cb(void *data UNUSED, Ecore_Thread *th UNUSED) {
   Bks_Thread_Data *th_data = (Bks_Thread_Data*)data;
   Bks_Model_User_Account *current_user;

   //Since we don't pass the list, we must free it, if exists
   if (!th_data->list) { // list was created
      EINA_LIST_FREE(th_data->list, current_user) {
         bks_model_user_account_free(current_user);
      }
   }
   free((Bks_Thread_Data*)th_data);
   fprintf(stderr,"Loading user accounts canceled\n");
   bks_controller_model_user_accounts_get_finished_cb(NULL, BKS_MODEL_THREAD_ERROR);
}



Ecore_Thread *_bks_model_user_accounts_get(unsigned int limit) {

   Bks_Thread_Data *data;
   data = calloc(1, sizeof(Bks_Thread_Data));
   data->list = NULL;
   data->limit = limit;

   if (limit == 0) {     
      return ecore_thread_run(_bks_model_user_accounts_get_cb, _bks_model_user_accounts_get_finished_cb, _bks_model_user_accounts_get_canceled_cb, data);
   } else {
      fprintf(stderr, "Recent user accounts UNUSED\n");
      return NULL;
   }
}

