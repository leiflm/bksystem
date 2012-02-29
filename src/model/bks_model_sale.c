//      bks_model_sale.c
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

#include <sqlite3.h>
#include "Bks_Model_Sale.h"

Bks_Model_Sale *bks_model_sale_new(sqlite3_uint64 uid, sqlite3_uint64 EAN) {
	
	Bks_Model_Sale *sale_ptr;
	sale_ptr = malloc(sizeof(struct _Bks_Model_Sale));
	sale_ptr->uid=uid;
	sale_ptr->EAN=EAN;
	sale_ptr->price=0;
	sale_ptr->status=BKS_MODEL_SALE_UNFINISHED;
}


void bks_model_sale_free(struct Bks_model_sale *sale_ptr) {	
	
	if (sale_ptr != NULL) {
		free(sale_ptr->firstname);
		free(sale_ptr->lastname);
		free(sale_ptr->productname);
		free(sale_ptr->timestamp);
		free(sale_ptr);
		sale_ptr = NULL;
	}
}
