#ifndef __BKS_MODEL_SQL_H__
#define __BKS_MODEL_SQL_H__

/**
 * @brief Commits a sale to the database
 *
 * @return Bks_Error indicator
 */
int _bks_model_sql_commit_sale(const Bks_Model_Sale *sale);

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @return Eina_List* of user_accounts sorted by sales volume since last bill
 */
Eina_List* _bks_model_sql_recent_user_accounts_get(const unsigned int limit); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @return Eina_List* of all user_accounts
 */
Eina_List* _bks_model_sql_user_accounts_get(void); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @return Eina_List*
 */
Eina_List* _bks_model_sql_products_get(void); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @return Eina_List*
 */
Eina_List* _bks_model_sql_recent_products_get(const unsigned int limit); 

/**
 * @brief timestring of current local time in required format
 *
 * @return timestring of current local time in required format
 */
char* bks_model_current_time_string_get(void);

/**
 * @brief creates a new database with final settlement for all and number of bought products
 *
 * @return success indicator
 */
int _bks_model_sql_create_bill_table(void);

#endif
