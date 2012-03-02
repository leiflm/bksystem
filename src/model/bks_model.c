//      bks_model.c
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

#include <Eina.h>
#include <Ecore.h>

#include "Bks_Types.h"
#include "Bks_System.h"
#include "Bks_Model.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_Threading.h"
#include "Bks_Error.h"

// Interface functions

void bks_model_init(void) {
   eina_lock_new(&mdl.lock);
   mdl.user_accounts = NULL;
   mdl.recent_user_accounts = NULL;
   mdl.products = NULL;
   mdl.favorite_products = NULL;
   _bks_model_products_get(6);
   _bks_model_user_accounts_get(6);
   //_bks_model_data_get();
}

void bks_model_shutdown(void) {

   Bks_Model_User_Account *user_data;
   Bks_Model_Product *product_data;

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
   eina_lock_free(&mdl.lock);
}


Eina_List* bks_model_user_accounts_get(unsigned int limit) {
   Eina_List *list = NULL, *rest = NULL;
   if (limit == 0) {
      list = eina_list_clone(mdl.user_accounts);
   } else {
      list = eina_list_clone(mdl.recent_user_accounts);
      limit = (limit > eina_list_count(list)) ? eina_list_count(list) : limit;
      list = eina_list_split_list(list, eina_list_nth_list(list, (limit - 1)), &rest);
      eina_list_free(rest);
   }
   return list;
}

Eina_List* bks_model_products_get(unsigned int limit) {
   Eina_List *list = NULL, *rest = NULL;
   if (limit == 0) {
      list = eina_list_clone(mdl.products);
   } else {
      list = eina_list_clone(mdl.favorite_products);
      limit = (limit > eina_list_count(list)) ? eina_list_count(list) : limit;
      list = eina_list_split_list(list, eina_list_nth_list(list, (limit - 1)), &rest);
      eina_list_free(rest);
   }
   return list;
}

void bks_model_commit_sale(const Bks_Model_Sale *sale) {
   
   _bks_model_commit_sale(sale); 

   }

const Eina_List* bks_model_sales_from_user_since(const sqlite3_uint64 uid, const char *since) {
   
   return NULL;
}

double bks_model_user_balance_get(const sqlite3_uint64 uid, const char *since) {
   return 0;
}

void bks_model_create_bill_table() {
}
