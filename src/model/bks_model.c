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

#include "bks_model.h"
#include "bks_model_user_account.h"
#include "bks_model_product.h"


const char *database = BKSYSTEMDB;



free_bks_model_userAccount(struct Bks_model_user_account *account_ptr) {	
	
	if (account_ptr != NULL) {
		free(account_ptr->firstname);
		free(account_ptr->lastname);
		free(account_ptr->image);
		free(account_ptr);
		account_ptr = NULL;
	}
}

free_bks_model_product(struct Bks_model_product *prod_ptr) {
	
	if (prod_ptr != NULL) {
		free(prod_ptr->name);
		free(prod_ptr->image);
		free(prod_ptr);
		prod_ptr = NULL;
	}
}

free_bks_model_sale(struct Bks_model_sale *sale_ptr) {	
	
	if (sale_ptr != NULL) {
		free(sale_ptr->firstname);
		free(sale_ptr->lastname);
		free(sale_ptr->productname);
		free(sale_ptr->timestamp);
		free(sale_ptr);
		sale_ptr = NULL;
	}
}


// Interface functions
// data retrieval
Eina_List* bks_model_recent_user_accounts_get(int n) {
	
	struct Bks_model_user_account *ptr_current_user = NULL;
	char *select_query ="SELECT user_accounts.uid,firstname,lastname,status,placement FROM user_accounts, recent_user_accounts WHERE user_accounts.uid=recent_user_accounts.uid ORDER BY placement";
    char *name;
    sqlite3 *pDb;
	sqlite3_stmt *stmt; 
    Eina_List *list= NULL;   
    int retval,i;
    
	eina_init(); 
    
    retval = sqlite3_open(database, &pDb);
	if( retval !=SQLITE_OK){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return NULL;
	}
	
    retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return NULL;
	}
     
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		i++;
        ptr_current_user = malloc(sizeof(struct Bks_model_user_account));
        
        ptr_current_user->uid = sqlite3_column_int64(stmt,0); 
        name = (char*)sqlite3_column_text(stmt,1);
        ptr_current_user-> firstname = (char*)malloc(strlen(name));
        strcpy(ptr_current_user->firstname, name);
        
        name = (char*)sqlite3_column_text(stmt,2);
        ptr_current_user-> lastname = (char*)malloc(strlen(name));
        strcpy(ptr_current_user->lastname, name);
        
        ptr_current_user->status = sqlite3_column_int64(stmt,3);
        list = eina_list_append(list, ptr_current_user);
        
        ptr_current_user->image = NULL; 
        if (i >=n) break; // We only want at most n Rows
    }
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\n", sqlite3_errmsg(pDb));	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, Error: %s\n", sqlite3_errmsg(pDb));
	}
	return list;
	
	
	
}

Eina_List* bks_model_user_accounts_get() {
	
	struct Bks_model_user_account *ptr_current_user = NULL;
	char *select_query ="SELECT uid,firstname,lastname,status FROM user_accounts ORDER BY lastname,firstname";
    char *name;
    sqlite3 *pDb;
	sqlite3_stmt *stmt; 
    Eina_List *list= NULL;   
    int retval;
    
	eina_init(); 
    
    retval = sqlite3_open(database, &pDb);
	if( retval !=SQLITE_OK){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return NULL; 
	}
	
    retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return NULL;
	}
     
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        ptr_current_user = malloc(sizeof(struct Bks_model_user_account));
        
        ptr_current_user->uid = sqlite3_column_int64(stmt,0); 
        name = (char*)sqlite3_column_text(stmt,1);
        ptr_current_user-> firstname = (char*)malloc(strlen(name));
        strcpy(ptr_current_user->firstname, name);
        
        name = (char*)sqlite3_column_text(stmt,2);
        ptr_current_user-> lastname = (char*)malloc(strlen(name));
        strcpy(ptr_current_user->lastname, name);
        
        ptr_current_user->status = sqlite3_column_int64(stmt,3);
        list = eina_list_append(list, ptr_current_user);
        
        ptr_current_user->image = NULL;
    }
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\n", sqlite3_errmsg(pDb));	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, Error: %s\n", sqlite3_errmsg(pDb));
	}
	return list;

}	
	
Eina_List* bks_model_favorite_products_get(int n) {
	struct Bks_model_product *ptr_current_product = NULL;
	char *select_query ="SELECT products.EAN,name,price,placement FROM products, favorite_products WHERE products.EAN=favorite_products.EAN ORDER BY placement";
    char *name;
    sqlite3 *pDb;
	sqlite3_stmt *stmt; 
    Eina_List *list= NULL;   
    int retval,i;
    
	eina_init(); 
    
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb); 
		return NULL;
	}
	retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return NULL;
	}
     
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		i++;
        ptr_current_product = malloc(sizeof(struct Bks_model_product));
        
        ptr_current_product->EAN = sqlite3_column_int64(stmt,0); 
        
        name = (char*)sqlite3_column_text(stmt,1);
        ptr_current_product-> name = (char*)malloc(strlen(name));
        strcpy(ptr_current_product->name, name);
        
        ptr_current_product->price = sqlite3_column_double(stmt,2);
        ptr_current_product->image=NULL;
        list = eina_list_append(list, ptr_current_product);
        if (i >=n) break; // We only want at most n Rows
    }
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
	}
	return list;
	
	}
		
Eina_List* bks_model_products_get() {
	
	struct Bks_model_product *ptr_current_product = NULL;
	char *select_query ="SELECT EAN,name,price FROM products ORDER BY name";
    char *name;
    sqlite3 *pDb;
	sqlite3_stmt *stmt; 
    Eina_List *list= NULL;   
    int retval;
    
	eina_init(); 
    
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	
    retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
     
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        ptr_current_product = malloc(sizeof(struct Bks_model_product));
        
        ptr_current_product->EAN = sqlite3_column_int64(stmt,0); 
        
        name = (char*)sqlite3_column_text(stmt,1);
        ptr_current_product-> name = (char*)malloc(strlen(name));
        strcpy(ptr_current_product->name, name);
        
        ptr_current_product->price = sqlite3_column_double(stmt,2);
        ptr_current_product->image=NULL;
        list = eina_list_append(list, ptr_current_product);
        
    }
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
	}
	return list;
}

// buying
int bks_model_make_sale(sqlite3_uint64 uid, sqlite3_uint64 EAN) {
		
	char *select_query ="SELECT EAN,price FROM products WHERE EAN = ?1";
	char *insert_query ="INSERT INTO sales (userid, product,price,timestamp) VALUES (?1,?2,?3,datetime('now'))"; //userid product price timestamp 
    sqlite3 *pDb;
	sqlite3_stmt *stmt;
	sqlite3_stmt *insert_stmt;   
    int retval;
    double price;
    char *currentTime=NULL;
    
    // first get the price of the product
    retval = sqlite3_open(database, &pDb);
	if(retval != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);  return -1;
	}
	
	// prepare SQL statement
    retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if (retval != SQLITE_OK) {
		fprintf(stderr, "prepare SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);  return -1;
	}
	// Bind product number to SQL-Statement, on failure return -1
	sqlite3_bind_double(stmt,1,EAN);
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		price = sqlite3_column_double(stmt,1);	
	} else {
		fprintf(stderr,"couldnt find product: %llu\n", EAN);
		bks_model_clean_up_DB(pDb,stmt);
		return -1;
	}
	// Deallocate Statment
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr, "finalize SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);
		return -1; 
	}	

	// Now prepare SQL-Statement for inserting in table "sales" 
	retval = sqlite3_prepare_v2(pDb,insert_query,-1,&insert_stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "prepare SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);  return -1;
	}

	retval = sqlite3_bind_int64(insert_stmt, 1, uid);
	retval = sqlite3_bind_int64(insert_stmt, 2, EAN);
	retval = sqlite3_bind_double(insert_stmt, 3, price);
	 
	// Performing INSERT
	retval = sqlite3_step(insert_stmt);
	if (retval != SQLITE_DONE) {
		fprintf(stderr, "Step SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		bks_model_clean_up_DB(pDb,stmt);
		return -1;
	}
	// Deallocate Statment
	retval = sqlite3_finalize(insert_stmt);
	if (retval != SQLITE_OK) {
		fprintf(stderr, "finalize SQL error:  %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval); 
		sqlite3_close(pDb);
		return -1;
	}
	// Close DB
	retval = sqlite3_close(pDb);
	if (retval != SQLITE_OK) {
		fprintf(stderr, "close SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval); 
		return -1;
	}
	return 0;
}

// service
Eina_List* bks_model_sales_from_user_since(sqlite3_uint64 uid,const char *timestamp) {

	struct Bks_model_sale *ptr_current_sale = NULL;
	char *select_query ="SELECT uid,firstname,lastname,status,EAN,name,price,timestamp FROM (((SELECT uid,firstname,lastname,status FROM user_accounts WHERE uid = ?2)INNER JOIN (SELECT userid,product,price,timestamp FROM sales WHERE timestamp > datetime(?1)) ON uid=userid ) INNER JOIN (SELECT EAN,name FROM products) ON EAN = product)ORDER BY uid,product";
    char *tmp;
    sqlite3 *pDb;
	sqlite3_stmt *stmt; 
    Eina_List *list = NULL;   
    int retval;
    
	eina_init(); 
    // Open DB
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s \nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);
		return NULL;
	}
	// Prepare SQL Statement
	retval = sqlite3_prepare_v2(pDb,select_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);
		return NULL; 
	}
	// Bind values to statement
    retval = sqlite3_bind_text(stmt,1,timestamp,-1,SQLITE_TRANSIENT);
    if (retval != SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval);
		bks_model_clean_up_DB(pDb,stmt);
		return NULL;	 
    }
    
    retval = sqlite3_bind_int64(stmt,2,uid);
    if (retval != SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n", retval);
		bks_model_clean_up_DB(pDb, stmt);
		return NULL; 
    }
    // For each row copy values into struct BKS_model_sale, alloc mem
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        ptr_current_sale = malloc(sizeof(struct Bks_model_sale));
        ptr_current_sale->uid = sqlite3_column_int64(stmt,0); 
        
        tmp = (char*)sqlite3_column_text(stmt,1);
        ptr_current_sale-> firstname = (char*)malloc(strlen(tmp));
        strcpy(ptr_current_sale->firstname, tmp);
        
        tmp = (char*)sqlite3_column_text(stmt,2);
        ptr_current_sale-> lastname = (char*)malloc(strlen(tmp));
        strcpy(ptr_current_sale->lastname, tmp);
        
        ptr_current_sale->status = sqlite3_column_int64(stmt,3);
		
		ptr_current_sale->EAN = sqlite3_column_int64(stmt,4);
        
        tmp = (char*)sqlite3_column_text(stmt,5);
        ptr_current_sale-> productname = (char*)malloc(strlen(tmp));
        strcpy(ptr_current_sale->productname, tmp);
        
        ptr_current_sale->price = sqlite3_column_double(stmt,6);
        
        tmp = (char*)sqlite3_column_text(stmt,7);
        ptr_current_sale-> timestamp = (char*)malloc(strlen(tmp));
        strcpy(ptr_current_sale->timestamp, tmp);
        
        // append struct to list
        list = eina_list_append(list, ptr_current_sale);
        }
	
	// Cleanup
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
	}
	return list;
}

double bks_model_calculate_sum_from_user_since(sqlite3_uint64 uid,const char *timestamp) {
	
	sqlite3 *pDb;
	sqlite3_stmt *stmt;
	char *query ="SELECT userid,price FROM sales WHERE userid = ?1 AND timestamp > datetime(?2)"; 
	int retval,i=0;
	double sum =0.0;
    retval = sqlite3_open(database, &pDb);
	if(retval != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);  return -1;
	}
	retval = sqlite3_prepare_v2(pDb,query,-1,&stmt,0);
	if (retval != SQLITE_OK) {
		fprintf(stderr, "prepare SQL error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
		sqlite3_close(pDb);  return -1;
	}
	// Bind product number and timestamp to SQL-Statement
	sqlite3_bind_int64(stmt,1,uid);
	sqlite3_bind_text(stmt,2,timestamp,-1,SQLITE_TRANSIENT);
	
	//Calculate sum
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		sum += sqlite3_column_double(stmt,1);//fprintf(stderr,"durchgänge sum:%d\n",i++);
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

// final settlement
int bks_model_create_bill_table() {
	
	char *startingFrom=EVER; 
	char *currentTime=bks_model_current_time_string_get();
	printf("\ncreating bill...\nfrom: %s\nuntil: %s\n\n",startingFrom,currentTime);

	// create Bill Table
	bks_model_create_bill_tableSinceUntil(startingFrom,currentTime);
	
	// setRecentUserAccounts()
	printf("trying to set recentuseraccount\n");
	if (bks_model_recent_user_accounts_set(startingFrom,currentTime)==SQLITE_OK) {
		printf("successfully set recent_user_accounts.\n");
	}
	
	// setFavoriteProducts()
	printf("trying to set favorite products\n");
	if (bks_model_favorite_products_get(startingFrom,currentTime)==SQLITE_OK) {
		printf("successfully set favorite products.\n");
	}
	
	free(currentTime);
	}



int bks_model_create_bill_table_since_until(const char *timestampSince,const char *timestampUntil) {
	
	printf("reading sales...\n");
	
	struct sale *ptr_current_sale;
	char *query ="SELECT userid, firstname, lastname, round(sum(round(price,2)),2) AS \"sum\" FROM ((SELECT uid,firstname,lastname FROM user_accounts ORDER BY lastname,firstname) INNER JOIN (SELECT userid, product, price,timestamp FROM sales WHERE timestamp>datetime(?1) AND timestamp <datetime(?2)) ON uid =userid) GROUP BY uid ORDER BY lastname,firstname";
    char *tmp;
    sqlite3 *pDb,*pDbNew;
	sqlite3_stmt *stmt,*stmt_create,*stmt_insert; 
    Eina_List *list= NULL;   
    int retval,i;
    
	eina_init(); 
    // Open bksystem DB
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	// Prepare Query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	// bind arguments to query
    retval = sqlite3_bind_text(stmt,1,timestampSince,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval); 
    }
    retval = sqlite3_bind_text(stmt,2,timestampUntil,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval);
	}
	// creating new DB BILLDB_date.sqlite 
	char *createquery ="CREATE TABLE IF NOT EXISTS \"main\".\"Bill\" (\"uid\" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof(\"uid\") = 'integer') , \"firstname\" VARCHAR, \"lastname\" VARCHAR, \"sum\" FLOAT)";
	char *insertquery ="INSERT OR REPLACE INTO Bill (uid,firstname,lastname,sum) VALUES (?1,?2,?3,?4)";
	
	char newDBfilename[BILLDBLENGTH];
	
	strcpy(newDBfilename,BILLDB);
	strncat(newDBfilename,timestampUntil,10);
	strcat(newDBfilename,".sqlite");
	printf("filename:%s...\n",newDBfilename);
	retval = sqlite3_open(newDBfilename, &pDbNew);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDbNew));
		sqlite3_close(pDbNew);
	}
	// prepare create table
	retval = sqlite3_prepare_v2(pDbNew,createquery,-1,&stmt_create,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDbNew));
		sqlite3_close(pDbNew);
	}
	// create table
	retval = sqlite3_step(stmt_create);

	if( retval != SQLITE_DONE){
		fprintf(stderr, "Can't create new table: %s,%d\n", sqlite3_errmsg(pDbNew),retval);

	} else {fprintf(stderr,"Created DB sucessfully....\n");}
	if (sqlite3_finalize(stmt_create) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDbNew), retval);
	}
	// prepare insertion
	retval = sqlite3_prepare_v2(pDbNew,insertquery,-1,&stmt_insert,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDbNew));
		sqlite3_close(pDbNew);
	}
	
	//bks_model_print_all_data(stmt);
	retval=sqlite3_step(stmt); 
	i=0;
	while (retval== SQLITE_ROW) {
		i++;
		// check Data in querry and asign to insertquery
		
		retval =sqlite3_bind_int64(stmt_insert,1,sqlite3_column_int64(stmt,0));
		if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding uid failed:%d \n",retval); 
		}
		sqlite3_bind_text(stmt_insert,2,sqlite3_column_text(stmt,1),-1,SQLITE_TRANSIENT);
		if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding firstname failed:%d \n",retval); 
		}	
		sqlite3_bind_text(stmt_insert,3,sqlite3_column_text(stmt,2),-1,SQLITE_TRANSIENT);
		if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding lastname failed:%d \n",retval); 
		}	
		sqlite3_bind_double(stmt_insert,4,sqlite3_column_double(stmt,3));
		if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding sum failed:%d \n",retval); 
		}
		// do insert
		retval = sqlite3_step(stmt_insert);


		// reset insert statment to do new bindings
		sqlite3_reset(stmt_insert);retval=sqlite3_step(stmt);
	} 
	if (i==0) { fprintf(stderr,"Error step:%d\n", retval);}
	if (sqlite3_finalize(stmt_insert) != SQLITE_OK) {
	fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDbNew), retval);	
	}
	if (sqlite3_close(pDbNew) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDbNew), retval);
	}
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);
	}
	return 0;
}

int bks_model_recent_user_accounts_set(const char *timestampSince,const char *timestampUntil) { 
	
	char *delete_query ="DELETE FROM recent_user_accounts";
	char *update_query ="UPDATE sqlite_sequence SET seq=0 WHERE name = \"recent_user_accounts\"";
	char *insert_query ="INSERT INTO recent_user_accounts (uid) SELECT userid FROM (SELECT userid, round(sum(round(price,2)),2) AS sum FROM  (SELECT userid, product, price,timestamp FROM sales WHERE timestamp>datetime(?1) AND timestamp<datetime(?2)) GROUP BY userid ORDER BY sum desc)";

	sqlite3 *pDb;
	sqlite3_stmt *stmt;   
    int retval;

    // Open bksystem DB
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	// Prepare Query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,delete_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	retval = sqlite3_step(stmt);
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
	fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	
	// Prepare update_query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,update_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	retval = sqlite3_step(stmt);
 
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
	fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	
	// Prepare insert_query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,insert_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	
	// bind arguments to query 
    retval = sqlite3_bind_text(stmt,1,timestampSince,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval); 
    }
    retval = sqlite3_bind_text(stmt,2,timestampUntil,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval);
	}
	retval = sqlite3_step(stmt);
	
	return bks_model_clean_up_DB(pDb,stmt);

}

int bks_model_favorite_products_set(const char *timestampSince,const char *timestampUntil) { 
	
	char *delete_query ="DELETE FROM favorite_products";
	char *update_query ="UPDATE sqlite_sequence SET seq=0 WHERE name = \"favorite_products\"";
	char *insert_query ="INSERT INTO favorite_products (EAN) SELECT product FROM (SELECT product, count(product) AS num FROM  (SELECT product, timestamp FROM sales WHERE timestamp>datetime(?1) AND timestamp <datetime(?2))  GROUP BY product ORDER BY num desc)";

	sqlite3 *pDb;
	sqlite3_stmt *stmt;   
    int retval;

    // Open bksystem DB
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	// Prepare delete_query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,delete_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	// calculate query 
	retval = sqlite3_step(stmt);
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
	fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	
	// Prepare update_query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,update_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	retval = sqlite3_step(stmt);
	if (sqlite3_finalize(stmt) != SQLITE_OK) {
	fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	
	// Prepare insert_query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,insert_query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
	}
	
	// bind arguments to query 
    retval = sqlite3_bind_text(stmt,1,timestampSince,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval); 
    }
    retval = sqlite3_bind_text(stmt,2,timestampUntil,-1,SQLITE_TRANSIENT);
    if (retval!= SQLITE_OK) {
		fprintf(stderr,"Binding date failed:%d \n",retval);
	}
	retval = sqlite3_step(stmt);
	
	return bks_model_clean_up_DB(pDb,stmt);

}



//Helper functions
char* bks_model_current_time_string_get() { // not optimal, user has to user a buffer
	time_t *time_t_ptr;
	time_t_ptr = malloc(sizeof(time_t));
	time(time_t_ptr);
	struct tm *time_tm_ptr =(struct tm*)malloc(sizeof(struct tm));
	time_tm_ptr = localtime(time_t_ptr);
	char* buffer=malloc(20);
	strftime(buffer,20,"%Y-%m-%d %X",time_tm_ptr);
	free(time_t_ptr);
	return buffer;

}

//Evaluate SQL Statements and print on console
void bks_model_print_all_data(sqlite3_stmt *stmt) {	
	sqlite3_reset(stmt);	 
    int cols = sqlite3_column_count(stmt);
    printf("table with %d columns\n", cols);
			 
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int col, ncol;
        
        for (col = 0; col < cols; col++) {
            sqlite3_uint64 uint_val;
            double dbl_val;
            const unsigned char *chr_ptr, *colname;
    
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
                    printf("BLOB, won't display it %s", colname);
                    break;
                case SQLITE_NULL:
                    printf("Watch out, we have a NULL value over here (%s)!", colname);
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



int bks_model_start_up_DB(const char* database, sqlite3 *pDb, const char *query, sqlite3_stmt *stmt) { 
	int retval;
	// Open bksystem DB
    retval = sqlite3_open(database, &pDb);
	if( retval ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
	return retval;
	}
	// Prepare Query for bksystem DB
	retval = sqlite3_prepare_v2(pDb,query,-1,&stmt,0);
	if( retval!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb); 
		return retval;
	}
	return retval;
}

int bks_model_clean_up_DB(sqlite3 *pDb,sqlite3_stmt *stmt) {
	int retval;
	retval = sqlite3_finalize(stmt);
	if (retval != SQLITE_OK) {
		fprintf(stderr,"Couldnt finalize statment, Error: %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval);	
	}
	if (sqlite3_close(pDb) != SQLITE_OK) {
		fprintf(stderr,"Couldnt close DB, %s\nErrno: %d\n", sqlite3_errmsg(pDb), retval); 
		return retval;
	}
	return retval;	
	
	
}
