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


#define BKSYSTEMDB "bksystem.sqlite"   //! (default) This sqlite database holds our users, products, sales etc., should reside in main directory
#define BKS_DB_PATH_FILE "db_path.txt" //! (default) This file holds path/name to database
#define EVER "0000-01-01 00:00:00"       //! Timestamp usable as parameter


//       Flags for
//
//      SQLITE_FLAGS=`pkg-config --cflags --libs sqlite3 `
//      EINA_FLAGS=`pkg-config --cflags --libs eina `





/**
 * @brief Model in MVC
 *
 * >struct Bks_Model is for internal use
 * @param lock database for exclusive access
 * @param DBPath contains location and filename of DB
 */
struct _Bks_Model {
   Eina_Lock lock;
   char *db_path;
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
 * @brief sets a new path for the database and saves it to a file for later use.
 */
void bks_model_controller_db_path_set(Eina_Stringshare *path);

/**
 * @brief Commits a List of sales to the database, blocking function
 *
 * @param Eina_List of Bks_Model_Sale: sale contains sale info (uid, EAN, price, status, etc.) with sale that should be performed on database
 *
 * @return Bks_Status indicator
 */
Bks_Status bks_model_controller_commit_sale(const Bks_Model_Sale *sales);

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of of all user_accounts
 * @return Bks_Status indicator
 */
Bks_Status bks_model_controller_user_accounts_get(Eina_List **user_accounts); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of all products
 * @return Bks_Status indicator
 */
Bks_Status bks_model_controller_products_fav_get(Eina_List **products, const unsigned int limit);

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of all products
 * @return Bks_Status indicator
 */
Bks_Status bks_model_controller_products_alpha_get(Eina_List **products); 


#endif
