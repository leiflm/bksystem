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
#include <Eet.h>



// Internal functions

// Path to database
static char* bks_model_get_path(void) {

   char *path;
   char *default_path = BKSYSTEMDB;
   size_t len;
      // set default path and name
      len = strlen(BKSYSTEMDB) + 256;
      path = malloc(len);
      snprintf(path, len, "file:/home/matthias/Projekte/bksystem/build/%s",  default_path);      
   return path;
   }

// Interface functions
void bks_model_set_path(char *path) {

   if (!mdl.db_path) {
      free(mdl.db_path);
   }
   mdl.db_path = path;
   _bks_model_save_path(path);
}

void bks_model_init(void) {
   eina_lock_new(&mdl.lock);

   mdl.db_path = bks_model_get_path();
   fprintf(stderr, "Path: %s", mdl.db_path);
}

void bks_model_shutdown(void) {

   eina_lock_free(&mdl.lock);
}

void bks_model_user_accounts_get(unsigned int limit) {

   _bks_model_user_accounts_get(limit);

}

void bks_model_products_get(unsigned int limit) {

   _bks_model_products_get(limit);

}

void bks_model_commit_sale(const Bks_Model_Sale *sale) {
   
   _bks_model_commit_sale(sale); 

   }

const Eina_List* bks_model_sales_from_user_since(const int uid UNUSED, const char *since UNUSED) {
   
   return NULL;
}

double bks_model_user_balance_get(const int uid UNUSED, const char *since UNUSED) {
   return 0;
}

void bks_model_create_bill_table(void) {
}
