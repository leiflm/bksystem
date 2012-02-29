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

#include <sqlite3.h>
#include <Eina.h>



#include "Bks_Types"
#include "Bks_Model.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Model_Private.h"


const char *database = BKSYSTEMDB;

// Interface functions
void bks_model_init(void);
void bks_model_shutdown(void);

void bks_model_commit_sale(const Bks_Model_Sale *sale) {
	
}


Eina_List* bks_model_user_accounts_get(const unsigned int limit) {

	return _bks_model_recent_user_accounts_get(limit);
}



Eina_List* bks_model_products_get(const unsigned int limit);
Eina_List* bks_model_sales_from_user_since(const sqlite3_uint64 uid, const char *since);
double bks_model_user_balance_get(const sqlite3_uint64 uid, const char *since);
void bks_model_create_bill_table();


