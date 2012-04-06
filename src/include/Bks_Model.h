//      Bks_Model.h
//
//      Copyright 2011 Matthias Wauer <matthiaswauer@googlemail.com>
//
//      This program is free software; you can redistribute it and/or
//      it under the terms of the GNU General Public License as published
//      the Free Software Foundation; either version 2 of the License,
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public
//      along with this program; if not, write to the Free
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef __BKS_MODEL_H__
#define __BKS_MODEL_H__

#include <sqlite3.h>
#include <Eina.h>
#include "Bks_Types.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"


#define BKSYSTEMDB "bksystem.sqlite"   //! This sqlite database holds our users, products, sales etc., should reside in main directory
#define BILLDB "Abrechnung_"         //! Used for naming the sqlite database which is created for billing accounts (date will be appended)
#define BILLDBLENGTH 29               //! Should be strlen(BILLDB + 18) to alloc buffer for DB


#define EVER "0000-01-01 00:00:00"       //! Timestamp usable as parameter to


//       Flags for
//
//      SQLITE_FLAGS=`pkg-config --cflags --libs sqlite3 `
//      EINA_FLAGS=`pkg-config --cflags --libs eina `





/**
 * @brief Model in MVC
 *
 * >struct Bks_Model is for internal use
 * @param products list of Bks_Model_Product
 * @param user_accounts list of Bks_Model_User_Account
 */
struct _Bks_Model {
   Eina_List *products;
   Eina_List *favorite_products;
   Eina_List *user_accounts;
   Eina_List *recent_user_accounts;
   Eina_Lock lock;
};

/**
 * @brief initializes the central model.
 */
void bks_model_init(void);

/**
 * @brief shuts the central model down.
 */
void bks_model_shutdown(void);

/**
 * @brief Commits a sale to the database
 *
 */
void bks_model_commit_sale(const Bks_Model_Sale *sale);

/**
 * @brief get up to @p limit user accounts sorted by sales volume
 *
 * @param limit number of accounts to @p limit number of accounts (ordered by
 * sales volume). If 0, all accounts are returned.
 *
 * @return Pointer to Eina_List of struct Bks_Model_User_Account, NULL on
 * failure.
 */
Eina_List* bks_model_user_accounts_get(unsigned int limit);

/**
 * @brief get up to @p limit favorite products, ordered by sales volume
 *
 * @param limit Number of products returned, ordered by 'most often bought'.
 *
 * @return Eina_List* of struct Bks_Model_Product, NULL on
 *
 */
Eina_List* bks_model_products_get(unsigned int limit);

/**
 * @brief gets all products bought by user newer then @p since
 *
 * @param uid id of the user, the information is queried for.
 * @param since timestamp until sales are dated back to
 *
 * @return List of struct sale
 */
const Eina_List* bks_model_sales_from_user_since(const int uid, const char *since);

/**
 * @brief adds up the prices of sales/products bought by @p uid
 *
 * @param uid user that bought
 * @param since sales dating back to given timestamp.
 *
 * @return accumulated sum
 */
double bks_model_user_balance_get(const int uid, const char *since);

/**
 * @brief creates a new database with final settlement for all and number of bought products
 *
 * @return success indicator
 */
void bks_model_create_bill_table();
#endif
