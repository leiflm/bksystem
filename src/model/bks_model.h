//      bks_model.h
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

#ifndef __BKS_MODEL_H__
#define __BKS_MODEL_H__

#include <sqlite3.h>
#include <Eina.h>


#define BKSYSTEMDB "bksystem.sqlite"	//! This sqlite database holds our users, products, sales etc., should reside in main directory 
#define BILLDB "Abrechnung_"			//! Used for naming the sqlite database which is created for billing accounts (date will be appended)
#define BILLDBLENGTH 29					//! Should be strlen(BILLDB + 18) to alloc buffer for DB name


#define EVER "0000-01-01 00:00:00" 		//! Timestamp usable as parameter to functions


// 		Flags for compiling
//
//		SQLITE_FLAGS=`pkg-config --cflags --libs sqlite3 `
//		EINA_FLAGS=`pkg-config --cflags --libs eina `

/**
 * @brief All info about a sale
 * 
 * 	struct Bks_model_sale can hold all information associated with a sale derived from different tables
 * 
 * @param sqlite3_uint_64 user account ID, also primary key
 * @param firstname
 * @param lastname
 * @param status will be used to block user 
 * @param EAN product number
 * @param productname
 * @param price associated with the sale, not the product! Product price my have changed  
 * @param char *timestamp has to be of format "YYYY-MM-DD hh:mm:ss" 
 * 
 * */
 
	
	struct Bks_model_sale {
		sqlite3_uint64 uid;
		char *firstname;
		char *lastname;
		sqlite3_uint64 status;
		sqlite3_uint64 EAN;
		char * productname;
		double price;
		char *timestamp;
	};

/**
 * @brief frees struct memory and NULLs pointers
 * 
 * 	frees memory occupied by struct Bks_model WARNING: Unused pointers must be NULL  
 * 
 * @param pointer to struct Bks_model_sale, NULL on failure
 * 
 * */ 

free_bks_model_sale(struct Bks_model_sale *sale_ptr);

/**
 * @brief frees struct memory and NULLs pointers
 * 
 * 	frees memory occupied by struct Bks_model_product WARNING: Unused pointers must be NULL  
 * 
 * @param pointer to struct Bks_model_product
 */

free_bks_model_product(struct Bks_model_product *prod_ptr);

/**
 * @brief frees struct memory and NULLs pointers
 * 
 * 	frees memory occupied by struct Bks_user_account WARNING: Unused pointers must be NULL  
 * 
 * @param pointer to struct Bks_model_user_account
 */

free_bks_model_user_account(struct Bks_model_user_account *user_account_ptr);

/**
 * @brief gets user accounts from database, ordered lastname and firstname
 *  
 * @return Pointer to Eina_List of struct Bks_model_user_account, NULL on failure
 */

Eina_List* bks_model_user_accounts_get();

/**
 * @brief get up to n user accounts sorted by sales volume
 * 
 * @param n number of accounts returned
 * 
 * @return Pointer to Eina_List of struct Bks_model_user_account, NULL on failure
 */

Eina_List* bks_model_recent_user_accounts_get(int n);

/**
 * @brief Gets list products from database ordered by productname
 *  
 * @return Eina_List* of struct Bks_model_product, NULL on failure
 */

Eina_List* bks_model_products_get();

/**
 * @brief get up to n favorite products, ordered by preference
 * 
 * @param n number of products returned
 * 
 * @return Eina_List* of struct Bks_model_product, NULL on failure
 * 
 */
						
Eina_List* bks_model_favorite_products_get(int n);			

/**
 * @brief writes a new column in the table sales with uid,EAN,price and a timestamp  
 * 
 * @param uid user that is buying a product  
 * @param EAN corresponding ID of the product 
 * 
 * @return 0 on success
 */

int bks_model_make_sale(sqlite3_uint64 uid, sqlite3_uint64 EAN);

/**
 * @brief gets all products bought by user newer then timestamp
 * 
 * @param uid
 * @param timestamp sales newer then timestamp
 * 
 * @return List of struct sale 
 */

Eina_List* bks_model_sales_from_user_since(sqlite3_uint64 uid,const char *timestamp); 

/**
 * @brief adds up the prices of products bought by user
 * 
 * @param user that bought products
 * @param only newer sales are accounted
 * 
 * @return sum of bought products
 */

double bks_model_calculate_sum_from_user_since(sqlite3_uint64 uid,const char *timestamp); 

/**
 * @brief creates a new database with final settlement for all and number of bought products 
 * 
 * @return 0 on success 
 */

int bks_model_create_bill_table(); 

/**
 * @brief timestring of current local time in required format
 * 
 * @return timestring of current local time in required format
 */

char* bks_model_current_time_string_get();

#endif
