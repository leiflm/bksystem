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

#include <sqlite3.h>
#include <Eina.h>

#include "Bks_Types.h"
#include "Bks_Model.h"
#include "Bks_System.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Status.h" 

#define SQLITE_OPEN_URI 0x00000040
// data retrieval

Bks_Status _bks_model_sql_products_alpha_get(Eina_List **list) {

   Bks_Model_Product *ptr_current_product = NULL;
   char *select_query = "SELECT id,name,price FROM products ORDER BY lower(name)";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   int retval;
   Bks_Status error = BKS_MODEL_SQLITE_ERROR;

   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY , NULL);
   if (retval ) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_db;
   }

   retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK ) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _finalize;
   }

   while (sqlite3_step(stmt) == SQLITE_ROW) {
  
      ptr_current_product = malloc(sizeof(Bks_Model_Product));

      ptr_current_product->EAN = sqlite3_column_int64(stmt, 0);

      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_product->name = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_product->name, name);

      ptr_current_product->price = sqlite3_column_double(stmt, 2);

      ptr_current_product->image.data = NULL;
      ptr_current_product->image.size = 0;
      ptr_current_product->status = sqlite3_column_int(stmt, 5);
      *list = eina_list_append(*list, ptr_current_product);
     
   }
_finalize:
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statement, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   } else {
      error = BKS_MODEL_SQLITE_OK;
   }
_close_db:
   sqlite3_close(pDb);
   return error;
}

Bks_Status _bks_model_sql_products_fav_get(Eina_List **list, const unsigned int limit) {
   Bks_Model_Product *ptr_current_product = NULL;
   char select_query[256];
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   int retval;
   unsigned int i = 0;
   void *image;
   int image_size;
   Bks_Status error = BKS_MODEL_SQLITE_ERROR;

   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY , NULL);
   if (retval ) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_db;
   }
   snprintf(select_query, 255, "SELECT products.id,name,price,placement,image,status FROM products, fav_products WHERE products.id=fav_products.product_id ORDER BY placement LIMIT %u",limit);   
   retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK ) {
      goto _finalize;
   }

   while (sqlite3_step(stmt) == SQLITE_ROW) {
      i++;
      ptr_current_product = malloc(sizeof(Bks_Model_Product));

      ptr_current_product->EAN = sqlite3_column_int64(stmt, 0);

      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_product->name = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_product->name, name);

      ptr_current_product->price = sqlite3_column_double(stmt, 2);

      // fetch image
      image = (void*)sqlite3_column_blob(stmt, 4);
      image_size = sqlite3_column_bytes(stmt, 4);
      ptr_current_product->image.data = (void*)malloc(image_size);
      if (ptr_current_product->image.data) { // check if we got the mem for the image
         memcpy(ptr_current_product->image.data, image, (size_t)image_size);
         ptr_current_product->image.size = image_size;
      } else {
         fprintf(stderr, "Not enough memory for image: %s\n", name);
         ptr_current_product->image.data = NULL;
         ptr_current_product->image.size = 0;
      }
      ptr_current_product->status = sqlite3_column_int(stmt, 5);

      *list = eina_list_append(*list, ptr_current_product);

   }
_finalize:
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr,"Couldnt finalize statement, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   } else {
      error = BKS_MODEL_SQLITE_OK;
   }
_close_db:
   sqlite3_close(pDb);
   return error;
}

Bks_Status _bks_model_sql_user_accounts_alpha_get(Eina_List **list) {

   Bks_Model_User_Account *ptr_current_user = NULL;
   char *select_query = "SELECT id,firstname,lastname,status FROM user_accounts ORDER BY lower(lastname),lower(firstname)";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   int retval;
   Bks_Status error = BKS_MODEL_SQLITE_ERROR;

   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY , NULL);
   if (retval ) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_db;
   }
   retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
   if (retval != SQLITE_OK ) {
      goto _finalize;
   }
   while (sqlite3_step(stmt) == SQLITE_ROW) {
      ptr_current_user = malloc(sizeof(Bks_Model_User_Account));

      ptr_current_user->uid = sqlite3_column_int(stmt, 0);
      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_user->firstname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->firstname, name);

      name = (char*)sqlite3_column_text(stmt, 2);
      ptr_current_user->lastname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->lastname, name);

      ptr_current_user->status = sqlite3_column_int(stmt, 3);

      ptr_current_user->image.data = NULL;
      ptr_current_user->image.size = 0;

      *list = eina_list_append(*list, ptr_current_user);
   }
_finalize:
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statement, Error: %s\n", sqlite3_errmsg(pDb));
   } else {
      error = BKS_MODEL_SQLITE_OK;
   }
_close_db:
   sqlite3_close(pDb);
   return error;
}

Bks_Status _bks_model_sql_user_accounts_fav_get(Eina_List **list, const unsigned int limit) {

   Bks_Model_User_Account *ptr_current_user = NULL;
   char select_query[256];
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   int retval;
   unsigned int i = 0;
   void *image;
   int image_size;
   Bks_Status error = BKS_MODEL_SQLITE_ERROR;

   snprintf(select_query, 255, "SELECT user_accounts.id,firstname,lastname,status,placement,image FROM user_accounts, fav_users WHERE user_accounts.id=fav_users.user_account_id ORDER BY placement LIMIT %u", limit);
   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY , NULL);
   if (retval ) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_db;
   }
    retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK) {
      goto _finalize;
   }
   while (sqlite3_step(stmt) == SQLITE_ROW) {
      i++;
      ptr_current_user = malloc(sizeof(Bks_Model_User_Account));
      ptr_current_user->uid = sqlite3_column_int(stmt, 0);

      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_user->firstname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->firstname, name);

      name = (char*)sqlite3_column_text(stmt, 2);
      ptr_current_user->lastname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->lastname, name);

      ptr_current_user->status = sqlite3_column_int(stmt, 3);
      // fetch image
      image = (void*)sqlite3_column_blob(stmt, 5);
      image_size = sqlite3_column_bytes(stmt, 5);
      ptr_current_user->image.data = (void*)malloc(image_size);
      if (ptr_current_user->image.data) { // check if we got the mem for the image
         memcpy(ptr_current_user->image.data, image, (size_t)image_size);
         ptr_current_user->image.size = image_size;
      } else {
         fprintf(stderr, "Not enough memory for image: %s\n", name);
         ptr_current_user->image.data = NULL;
         ptr_current_user->image.size = 0;
      }
      *list = eina_list_append(*list, ptr_current_user);

   }
_finalize:
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr,"Couldnt finalize statement, Error: %s\n", sqlite3_errmsg(pDb));
   } else {
      error = BKS_MODEL_SQLITE_OK;
   }
_close_db:
   sqlite3_close(pDb);
   return error;
}

// buying
Bks_Status _bks_model_sql_commit_sale(const Bks_Model_Sale *sales) {

   char *insert_query ="INSERT INTO sales (user_account_id, product_id,price,created_at) VALUES (?1,?2,?3,datetime('now','localtime'))"; //userid product price timestamp
   sqlite3 *pDb;
   sqlite3_stmt *insert_stmt;
   int retval;
   Bks_Model_User_Account *current_user;
   Eina_List *current_node;
   Bks_Status error = BKS_MODEL_SQLITE_ERROR;

   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI | SQLITE_OPEN_READWRITE, NULL);
   if(retval != SQLITE_OK) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_and_return;
   }
   // Now prepare SQL-Statement for inserting in table "sales"
   retval = sqlite3_prepare_v2(pDb,insert_query, -1, &insert_stmt, 0);
   if (retval != SQLITE_OK) {
      if (retval == SQLITE_BUSY) {
         error = BKS_MODEL_SQLITE_DATABASE_LOCKED;
         goto _close_and_return;
      } else {
         error = BKS_MODEL_SQLITE_ERROR;
         goto _close_and_return;
      }
   }
   retval = sqlite3_bind_int64(insert_stmt, 2, sales->EAN);
   retval = sqlite3_bind_double(insert_stmt, 3, sales->price);

   // Insert for every user in the user account list in sale
   EINA_LIST_FOREACH(sales->user_accounts, current_node, current_user) {

      retval = sqlite3_bind_int64(insert_stmt, 1, current_user->uid);

      // Performing INSERT
      retval = sqlite3_step(insert_stmt);
      if (retval != SQLITE_DONE) {
         error = BKS_MODEL_SQLITE_OPEN_ERROR;
         goto finalize_close_and_return;
      }
      sqlite3_reset(insert_stmt);
   }
   // Deallocate Statment
   retval = sqlite3_finalize(insert_stmt);
   if (retval != SQLITE_OK) {
      error = BKS_MODEL_SQLITE_WRITE_ERROR;
   } else {
      error = BKS_MODEL_SALE_DONE;
   }
   goto _close_and_return;

   finalize_close_and_return:
 //  fprintf(stderr, "%s: %s\n"
 //                    "\tSQL Errno: %d\n", BKS_ERROR_STRINGS[error], sqlite3_errmsg(pDb), retval);
   retval = sqlite3_finalize(insert_stmt);

   _close_and_return:
   if (retval != SQLITE_OK) {
      fprintf(stderr, "%s: %s\n"
                      "\tSQL Errno: %d\n", BKS_STATUS_STRINGS[error], sqlite3_errmsg(pDb), retval);
   }  else {
      error = BKS_MODEL_SQLITE_OK;
   }
   sqlite3_close(pDb);
   return error;
}

//// service
//Bks_Status _bks_model_sql_sales_from_user_since(Eina_List **list, int uid, const char *timestamp) {

   //Bks_Model_Sale *ptr_current_sale = NULL;
   //char *select_query ="SELECT uid,firstname,lastname,status,EAN,name,price,timestamp FROM (((SELECT uid,firstname,lastname,status FROM user_accounts WHERE uid = ?2)INNER JOIN (SELECT userid,product,price,timestamp FROM sales WHERE timestamp > datetime(?1)) ON uid=userid ) INNER JOIN (SELECT EAN,name FROM products) ON EAN = product)ORDER BY uid,product";
   //char *tmp;
   //sqlite3 *pDb;
   //sqlite3_stmt *stmt;
   //int retval;
   //Bks_Status error = BKS_MODEL_SQLITE_ERROR;

    //// Open DB
   //retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI & SQLITE_OPEN_READONLY, NULL);
   //if (retval) {
      //error = BKS_MODEL_SQLITE_OPEN_ERROR;
      //goto _close_db;
   //}
   //// Prepare SQL Statement
   //retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   //if (retval != SQLITE_OK) {
      //goto _finalize;
   //}
   //// Bind values to statement
   //retval = sqlite3_bind_text(stmt, 1, timestamp, -1, SQLITE_TRANSIENT);
   //if (retval != SQLITE_OK) {
      //goto _finalize;
   //}
   //retval = sqlite3_bind_int64(stmt, 2, uid);
   //if (retval != SQLITE_OK) {
      //goto _finalize;
   //}
   //// For each row copy values into struct BKS_model_sale, alloc mem
   //while (sqlite3_step(stmt) == SQLITE_ROW) {
      //ptr_current_sale = malloc(sizeof(Bks_Model_Sale));
      //ptr_current_sale->uid = sqlite3_column_int(stmt, 0);

      //tmp = (char*)sqlite3_column_text(stmt, 1);
      //ptr_current_sale->firstname = (char*)malloc(strlen(tmp + 1));
      //strcpy(ptr_current_sale->firstname, tmp);

      //tmp = (char*)sqlite3_column_text(stmt, 2);
      //ptr_current_sale->lastname = (char*)malloc(strlen(tmp + 1));
      //strcpy(ptr_current_sale->lastname, tmp);

      //ptr_current_sale->status = sqlite3_column_int(stmt, 3);

      //ptr_current_sale->EAN = sqlite3_column_int64(stmt, 4);

      //tmp = (char*)sqlite3_column_text(stmt, 5);
      //ptr_current_sale->productname = (char*)malloc(strlen(tmp + 1));
      //strcpy(ptr_current_sale->productname, tmp);

      //ptr_current_sale->price = sqlite3_column_double(stmt, 6);

      //tmp = (char*)sqlite3_column_text(stmt, 7);
      //ptr_current_sale->timestamp = (char*)malloc(strlen(tmp + 1));
      //strcpy(ptr_current_sale->timestamp, tmp);

      //// append struct to list
      //*list = eina_list_append(*list, ptr_current_sale);
   //}

   //// Cleanup
//_finalize:
   //if (sqlite3_finalize(stmt) != SQLITE_OK) {
      //fprintf(stderr, "Couldnt finalize statement, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   //} else {
      //error = BKS_MODEL_SQLITE_OK;
   //}
//_close_db:
   //sqlite3_close(pDb);
   //return error;
//}

double _bks_model_sql_calculate_sum_from_user_since(int uid, const char *timestamp) {

   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   char *query = "SELECT user_account_id,sum(price) FROM sales WHERE user_account_id = ?1 AND created_at > datetime(?2)";
   int retval;
   double sum = 0.0;
   retval = sqlite3_open_v2(mdl.db_path, &pDb, SQLITE_OPEN_URI & SQLITE_OPEN_READONLY, NULL);
   if (retval != SQLITE_OK) {
      sum = -1;
      goto _close_db;
   }
   retval = sqlite3_prepare_v2(pDb,query,-1,&stmt,0);
   if (retval != SQLITE_OK) {
      sum = -1;
      goto _finalize;
   }
   // Bind product number and timestamp to SQL-Statement
   sqlite3_bind_int64(stmt, 1, uid);
   sqlite3_bind_text(stmt, 2, timestamp, -1, SQLITE_TRANSIENT);

   //Calculate sum
   if (sqlite3_step(stmt) == SQLITE_ROW) {
      sum = sqlite3_column_double(stmt, 1);
   }
   // Deallocate Statment
_finalize:
   retval = sqlite3_finalize(stmt);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "finalize SQL error:  %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
_close_db:
   retval = sqlite3_close(pDb);
   return sum;
}
