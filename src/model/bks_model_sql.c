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
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Error.h"

const char *database = BKSYSTEMDB;

static int bks_model_clean_up_DB(sqlite3 *pDb, sqlite3_stmt *stmt);
//static void bks_model_print_all_data(sqlite3_stmt *stmt);


// data retrieval
Eina_List* _bks_model_sql_recent_user_accounts_get(const unsigned int limit) {

   Bks_Model_User_Account *ptr_current_user = NULL;
   char *select_query = "SELECT user_accounts.uid,firstname,lastname,status,placement,image FROM user_accounts, recent_user_accounts WHERE user_accounts.uid=recent_user_accounts.uid ORDER BY placement";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   Eina_List *list = NULL;
   int retval;
   unsigned int i = 0;
   void *image;
   int image_size;

   retval = sqlite3_open(database, &pDb);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
   }

    retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
   }

   while (sqlite3_step(stmt) == SQLITE_ROW) {
      i++;
      ptr_current_user = malloc(sizeof(Bks_Model_User_Account));

      ptr_current_user->uid = sqlite3_column_int64(stmt, 0);
      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_user->firstname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->firstname, name);

      name = (char*)sqlite3_column_text(stmt, 2);
      ptr_current_user->lastname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->lastname, name);

      ptr_current_user->status = sqlite3_column_int64(stmt, 3);

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

      list = eina_list_append(list, ptr_current_user);

      if (i >= limit) {// We only want at most limit Rows
         break;
      }
   }
   //bks_model_print_all_data(stmt);
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr,"Couldnt finalize statment, Error: %s\n", sqlite3_errmsg(pDb));
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr,"Couldnt close DB, Error: %s\n", sqlite3_errmsg(pDb));
   }
   return list;
}

Eina_List* _bks_model_sql_user_accounts_get(void) {

   Bks_Model_User_Account *ptr_current_user = NULL;
   char *select_query = "SELECT uid,firstname,lastname,status FROM user_accounts ORDER BY lastname,firstname";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   Eina_List *list = NULL;
   int retval;

   retval = sqlite3_open(database, &pDb);
   if (retval !=SQLITE_OK) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
   }

   retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
   if (retval != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
   }

   while (sqlite3_step(stmt) == SQLITE_ROW) {
      ptr_current_user = malloc(sizeof(Bks_Model_User_Account));

      ptr_current_user->uid = sqlite3_column_int64(stmt, 0);
      name = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_user->firstname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->firstname, name);

      name = (char*)sqlite3_column_text(stmt, 2);
      ptr_current_user->lastname = (char*)malloc((strlen(name) + 1));
      strcpy(ptr_current_user->lastname, name);

      ptr_current_user->status = sqlite3_column_int64(stmt, 3);

      ptr_current_user->image.data = NULL;
      ptr_current_user->image.size = 0;

      list = eina_list_append(list, ptr_current_user);
   }
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statment, Error: %s\n", sqlite3_errmsg(pDb));
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr, "Couldnt close DB, Error: %s\n", sqlite3_errmsg(pDb));
   }
   return list;
}

Eina_List* _bks_model_sql_favorite_products_get(const unsigned int limit) {
   Bks_Model_Product *ptr_current_product = NULL;
   char *select_query ="SELECT products.EAN,name,price,placement,image FROM products, favorite_products WHERE products.EAN=favorite_products.EAN ORDER BY placement";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   Eina_List *list= NULL;
   int retval;
   unsigned int i = 0;
   void *image;
   int image_size;

   retval = sqlite3_open(database, &pDb);
   if (retval ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
   }
   retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return NULL;
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

      list = eina_list_append(list, ptr_current_product);
      if (i >= limit) { // We only want at most limit Rows
         break;
      }
   }
   //bks_model_print_all_data(stmt);
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }

   return list;
}

Eina_List* _bks_model_sql_products_get(void) {

   Bks_Model_Product *ptr_current_product = NULL;
   char *select_query = "SELECT EAN,name,price FROM products ORDER BY name";
   char *name;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   Eina_List *list = NULL;
   int retval;

   retval = sqlite3_open(database, &pDb);
   if (retval ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
   }

   retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
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

      list = eina_list_append(list, ptr_current_product);
    }
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr, "Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   return list;
}

// buying
Bks_Error _bks_model_sql_commit_sale(const Bks_Model_Sale *sale) {

  // char *select_query ="SELECT EAN,price FROM products WHERE EAN = ?1";
   char *insert_query ="INSERT INTO sales (userid, product,price,timestamp) VALUES (?1,?2,?3,datetime('now'))"; //userid product price timestamp
   sqlite3 *pDb;
   //sqlite3_stmt *stmt;
   sqlite3_stmt *insert_stmt;
   int retval;
   //double price;
   Bks_Error error = BKS_MODEL_SQLITE_OK;

   // open DB
   retval = sqlite3_open(database, &pDb);
   if(retval != SQLITE_OK) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto _close_and_return;
   }
/*
   // first get the price of the product and prepare SQL statement
   retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
   if (retval != SQLITE_OK) {
      if (retval == SQLITE_BUSY) {
         error = BKS_MODEL_SQLITE_DATABASE_LOCKED;
         goto _close_and_return;
      } else {
         error = BKS_MODEL_SQLITE_ERROR;
         goto _close_and_return;
      }
   }
   // Bind product number to SQL-Statement
   sqlite3_bind_double(stmt,1,sale->EAN);
   if (sqlite3_step(stmt) == SQLITE_ROW) {
      price = sqlite3_column_double(stmt,1);
   } else {
      error = BKS_MODEL_SQLITE_ROW_MISSING;
      goto finalize_close_and_return;
   }
   // Deallocate Statment
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      error = BKS_MODEL_SQLITE_ERROR;
      goto _close_and_return;
   }
*/
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

   retval = sqlite3_bind_int64(insert_stmt, 1, sale->uid);
   retval = sqlite3_bind_int64(insert_stmt, 2, sale->EAN);
   retval = sqlite3_bind_double(insert_stmt, 3, sale->price);

   // Performing INSERT
   retval = sqlite3_step(insert_stmt);
   if (retval != SQLITE_DONE) {
      error = BKS_MODEL_SQLITE_OPEN_ERROR;
      goto finalize_close_and_return;
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
   fprintf(stderr, "%s: %s\n"
                     "\tSQL Errno: %d\n", BKS_ERROR_STRINGS[error], sqlite3_errmsg(pDb), retval);
   retval = sqlite3_finalize(insert_stmt);

   _close_and_return:
   fprintf(stderr, "%s: %s\n"
                     "\tSQL Errno: %d\n", BKS_ERROR_STRINGS[error], sqlite3_errmsg(pDb), retval);
   sqlite3_close(pDb);
   return error;
}

// service
Eina_List* _bks_model_sql_sales_from_user_since(sqlite3_uint64 uid,const char *timestamp) {

   Bks_Model_Sale *ptr_current_sale = NULL;
   char *select_query ="SELECT uid,firstname,lastname,status,EAN,name,price,timestamp FROM (((SELECT uid,firstname,lastname,status FROM user_accounts WHERE uid = ?2)INNER JOIN (SELECT userid,product,price,timestamp FROM sales WHERE timestamp > datetime(?1)) ON uid=userid ) INNER JOIN (SELECT EAN,name FROM products) ON EAN = product)ORDER BY uid,product";
   char *tmp;
   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   Eina_List *list = NULL;
   int retval;

    // Open DB
   retval = sqlite3_open(database, &pDb);
   if (retval) {
      fprintf(stderr, "Can't open database: %s \nErrno: %d\n", sqlite3_errmsg(pDb), retval);
      sqlite3_close(pDb);
      return NULL;
   }
   // Prepare SQL Statement
   retval = sqlite3_prepare_v2(pDb, select_query, -1, &stmt, 0);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
      sqlite3_close(pDb);
      return NULL;
   }
   // Bind values to statement
   retval = sqlite3_bind_text(stmt, 1, timestamp, -1, SQLITE_TRANSIENT);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "Binding date failed:%d \n", retval);
      bks_model_clean_up_DB(pDb, stmt);
      return NULL;
   }

   retval = sqlite3_bind_int64(stmt, 2, uid);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "Binding date failed:%d \n", retval);
      bks_model_clean_up_DB(pDb, stmt);
      return NULL;
   }
   // For each row copy values into struct BKS_model_sale, alloc mem
   while (sqlite3_step(stmt) == SQLITE_ROW) {
      ptr_current_sale = malloc(sizeof(Bks_Model_Sale));
      ptr_current_sale->uid = sqlite3_column_int64(stmt, 0);

      tmp = (char*)sqlite3_column_text(stmt, 1);
      ptr_current_sale->firstname = (char*)malloc(strlen(tmp + 1));
      strcpy(ptr_current_sale->firstname, tmp);

      tmp = (char*)sqlite3_column_text(stmt, 2);
      ptr_current_sale->lastname = (char*)malloc(strlen(tmp + 1));
      strcpy(ptr_current_sale->lastname, tmp);

      ptr_current_sale->status = sqlite3_column_int64(stmt, 3);

      ptr_current_sale->EAN = sqlite3_column_int64(stmt, 4);

      tmp = (char*)sqlite3_column_text(stmt, 5);
      ptr_current_sale->productname = (char*)malloc(strlen(tmp + 1));
      strcpy(ptr_current_sale->productname, tmp);

      ptr_current_sale->price = sqlite3_column_double(stmt, 6);

      tmp = (char*)sqlite3_column_text(stmt, 7);
      ptr_current_sale->timestamp = (char*)malloc(strlen(tmp + 1));
      strcpy(ptr_current_sale->timestamp, tmp);

      // append struct to list
      list = eina_list_append(list, ptr_current_sale);
   }

   // Cleanup
   if (sqlite3_finalize(stmt) != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr, "Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   return list;
}

double _bks_model_sql_calculate_sum_from_user_since(sqlite3_uint64 uid, const char *timestamp) {

   sqlite3 *pDb;
   sqlite3_stmt *stmt;
   char *query = "SELECT userid,price FROM sales WHERE userid = ?1 AND timestamp > datetime(?2)";
   int retval;
   double sum =0.0;
   retval = sqlite3_open(database, &pDb);
   if (retval != SQLITE_OK) {
   fprintf(stderr, "Can't open database: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   sqlite3_close(pDb);  return -1;
   }
   retval = sqlite3_prepare_v2(pDb,query,-1,&stmt,0);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "prepare SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
      sqlite3_close(pDb);  return -1;
   }
   // Bind product number and timestamp to SQL-Statement
   sqlite3_bind_int64(stmt, 1, uid);
   sqlite3_bind_text(stmt, 2, timestamp, -1, SQLITE_TRANSIENT);

   //Calculate sum
   while (sqlite3_step(stmt) == SQLITE_ROW) {
      sum += sqlite3_column_double(stmt, 1);//fprintf(stderr,"durchgänge sum:%d\n",i++);
   }
   // Deallocate Statment
   retval = sqlite3_finalize(stmt);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "finalize SQL error:  %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval); return -1;
   }
   retval = sqlite3_close(pDb);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "close SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval); return -1;
   }
   return sum;
}

//Helper functions
/*
static void bks_model_print_all_data(sqlite3_stmt *stmt) {
   sqlite3_reset(stmt);
   int cols = sqlite3_column_count(stmt);
   printf( "table with %d columns\n", cols);

      while (sqlite3_step(stmt) == SQLITE_ROW) {
      int col;

      for (col = 0; col < cols; col++) {
         sqlite3_uint64 uint_val;
         double dbl_val;
         const char *chr_ptr, *colname;
         colname = sqlite3_column_name(stmt,col);

            switch (sqlite3_column_type(stmt,col)) {
               case SQLITE_INTEGER:
                  uint_val =  sqlite3_column_int64(stmt,col);
                  printf("%s = %llu type:int64", colname, uint_val);
               break;
               case SQLITE_FLOAT:
                  dbl_val =  sqlite3_column_double(stmt,col);
                  printf("%s = %f type:double", colname,dbl_val);
               break;
               case SQLITE_TEXT:
                  chr_ptr = (const char*)sqlite3_column_text(stmt,col);
                  printf("%s = %s type:text", colname,chr_ptr);
               break;
               case SQLITE_BLOB:
                  printf("%s = BLOB, won't display it", colname);
               break;
               case SQLITE_NULL:
                  printf("%s = NULL! Watch out, we have a NULL value over here!", colname);
               break;
               default:
                  printf("Here is somthing I dont recognize on:%s\n", colname);
               //break;
         }

         printf("\n");
      }
      printf("\n");
   }
   printf("All rows finished\n");
   sqlite3_reset(stmt);
}
*/

int bks_model_start_up_DB(const char* db, sqlite3 *pDb, const char *query, sqlite3_stmt *stmt) {
   int retval;
   // Open bksystem DB
   retval = sqlite3_open(db, &pDb);
   if (retval) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
   return retval;
   }
   // Prepare Query for bksystem DB
   retval = sqlite3_prepare_v2(pDb, query, -1, &stmt, 0);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
      sqlite3_close(pDb);
      return retval;
   }
   return retval;
}

static int bks_model_clean_up_DB(sqlite3 *pDb, sqlite3_stmt *stmt) {
   int retval;
   retval = sqlite3_finalize(stmt);
   if (retval != SQLITE_OK) {
      fprintf(stderr, "Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
   }
   if (sqlite3_close(pDb) != SQLITE_OK) {
      fprintf(stderr, "Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
      return retval;
   }
   return retval;
}
