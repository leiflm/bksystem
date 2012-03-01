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

// Interface functions

void bks_model_init(void) {
	eina_lock_new(&mdl.lock);
	_bks_model_data_get();
	}

void bks_model_shutdown(void) {
	
	eina_lock_free(&mdl.lock);
	}


const Eina_List* bks_model_user_accounts_get(const unsigned int limit) {

	return mdl.user_accounts;
}

const Eina_List* bks_model_products_get(const unsigned int limit) {
	
	return mdl.products;
}

const Eina_List* bks_model_sales_from_user_since(const sqlite3_uint64 uid, const char *since) {
	
	return NULL;
	}

double bks_model_user_balance_get(const sqlite3_uint64 uid, const char *since) {
	return 0;
}

void bks_model_create_bill_table() {
	}
