//      Bks_Model_Sale.h
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

#ifndef __BKS_MODEL_SALE_H__
#define __BKS_MODEL_SALE_H__

#include <sqlite3.h>
#include <Eina.h>
#include "Bks_Types.h"
#include "Bks_Status.h"

/**
 * @brief All info about a sale
 *
 * struct Bks_Model_Sale can hold all information associated with a sale derived from different tables
 *
 * @param int user account ID, also primary key
 * @param firstname
 * @param lastname
 * @param status indicates success or failure of sale.
 * @param pid product ID
 * @param productname
 * @param price associated with the sale, not the product! Product price may have changed.
 * @param char *timestamp has to be of format "YYYY-MM-DD hh:mm:ss"
 *
 * */

struct _Bks_Model_Sale {
   Eina_List *user_accounts;
   Bks_Status status;
   Eina_List *products;
   char *timestamp;
};

/** @brief allocates a new Bks_Model_Sale element and initializes it with the
 * given parameters.
 * @param Eina list of user accounts that are buying a product
 * @param Eina list of products that are bought
 */
Bks_Model_Sale *bks_model_sale_new(Eina_List *user_accounts, Eina_List *products);

/**
 * @brief frees struct memory and NULLs
 *
 * frees memory occupied by struct Bks_model WARNING: Unused pointers must be NULL
 *
 * @param pointer to struct Bks_Model_Sale, NULL on
 *
 * */
void bks_model_sale_free(Bks_Model_Sale *sale_ptr);


#endif



