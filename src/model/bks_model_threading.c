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

#include <Ecore.h>
#include <Eina.h>

#include "Bks_Types"
#include "Bks_Model.h"
#include "Bks_Model_Sql"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"


// All Data
Ecore_Thread *_bks_model_data_get() {
	return ecore_thread_run(_bks_model_commit_sale_cb,_bks_model_sale_finished_cb,NULL, sale);
}

static void _bks_model_data_get_cb(void *data, Ecore_Thread *th) {
	
	Eina_List *products;
	Eina_List *user_accounts;
	
	eina_lock_take(&mdl->lock);
	products = _bks_model_products_get();
	user_accounts = _bks_model_user_accounts_get();
	if ((!products) || (!user_accounts)) {
		ecore_thread_cancel(th);
		eina_lock_release(&mdl->lock);
		return;
		}
	mdl->products = products;
	mdl->user_accounts = user_accounts;
	eina_lock_release(&mdl->lock);

	}

static void _bks_model_data_get_finished_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}

static void _bks_model_data_get_canceled_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}



// Sales
Ecore_Thread *_bks_model_commit_sale(const Bks_Model_Sale *sale) {
	return ecore_thread_run(_bks_model_commit_sale_cb,_bks_model_sale_finished_cb,NULL, sale);
}

static void _bks_model_commit_sale_cb(void *data, Ecore_Thread *th) {
	
	Bks_Model_Sale *sale;
	
	sale = data;
	if (!_bks_model_commit_sale(sale->uid,sale->EAN)) {
		sale->status = BKS_MODEL_SALE_UNFINISHED;
		ecore_thread_cancel(th);
		return;
		};
	sale->status = BKS_MODEL_SALE_DONE;
		
	}

static void _bks_model_sale_finished_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_sale_finished_cb(data);
	}

static void _bks_model_sale_canceled_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_sale_finished_cb(data);
	}

// Products
Ecore_Thread *_bks_model_products_get(int limit) {
	
	ecore_thread_run(_bks_model_products_get_cb, bks_model_products_get_finished_cb,bks_model_productes_get_canceled, &limit);
	
	}

static void _bks_model_products_get_cb(void *data, Ecore_Thread *th) {
	
	Eina_List *products;
	
	eina_lock_take(&mdl->lock);
	products = _bks_model_products_get();
	if (!products) {
		ecore_thread_cancel(th);
		eina_lock_release(&mdl->lock);
		return;
		}
	mdl->products =products;
	eina_lock_release(&mdl->lock);
	}

static void _bks_model_products_get_finished_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}	

static void _bks_model_products_get_canceled_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}

// User Accounts
Ecore_Thread *_bks_model_user_accounts_get(int limit) {
	
	ecore_thread_run(_bks_model_user_accounts_get_cb, bks_model_user_accounts_get_finished_cb,bks_model_user_accounts_get_canceled, &limit);
	
	}

static void _bks_model_user_accounts_get_cb(void *data, Ecore_Thread *th) {
	
	Eina_List *user_accounts;
	
	eina_lock_take(&mdl->lock);
	user_accounts = _bks_model_user_accounts_get();
	if (!user_accounts) {
		ecore_thread_cancel(th);
		eina_lock_release(&mdl->lock);
		return;
		}
	mdl-> user_accounts = user_accounts;
	eina_lock_release(&mdl->lock);
	}

static void _bks_model_user_accounts_get_finished_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}

static void _bks_model_user_accounts_get_canceled_cb(void *data, Ecore_Thread *th) {
	
	bks_controller_model_reloaded_cb(data);
	}

// Bill Creating
Ecore_Thread *_bks_model_create_bill_table() {

	ecore_thread_run(_bks_model_create_bill_table_cb, _bks_model_create_bill_table_finished_cb,bks_model_user_accounts_get_canceled, &limit);
	}

static void _bks_model_create_bill_table_cb(void *data, Ecore_Thread *th) {

	eina_lock_take(&mdl->lock);
	_bks_model_create_bill_table();
	_bks_model_data_get();
	eina_lock_release(&mdl->lock);
	}

static void _bks_model_create_bill_table_finished_cb(void *data, Ecore_Thread *th) {
	bks_controller_model_create_bill_table_cb();
	}

static void _bks_model_create_bill_table_canceled_cb(void *data, Ecore_Thread *th) {
	bks_controller_model_create_bill_table_cb();
	}

