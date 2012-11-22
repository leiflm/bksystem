#ifndef __BKS_MODEL_SQL_H__
#define __BKS_MODEL_SQL_H__

/**
 * @brief Commits a sale to the database
 *
 * @param Bks_Model_Sale sale contains sale info (uid, EAN, price, status, etc.) with sale that should be performed on database
 *
 * @return Bks_Status indicator
 */
Bks_Status _bks_model_sql_commit_sale(const Bks_Model_Sale *sales);

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of user_accounts sorted by sales volume since last bill
 * @param limit # of user accounts returned
 * @return Bks_Status indicator
 */
Bks_Status _bks_model_sql_recent_user_accounts_alpha_get(Eina_List **list, const unsigned int limit); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of of all user_accounts
 * @return Bks_Status indicator
 */
Bks_Status _bks_model_sql_user_accounts_alpha_get(Eina_List **list); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of all products
 * @return Bks_Status indicator
 */
Bks_Status _bks_model_sql_products_get(Eina_List **list); 

/**
 * @brief does SQL querry to sqlite3 database, blocking function
 *
 * @param Eina_List* list should be set to NULL and will after return contain a list of limit products with consumption
 * @param limit # of items return
 * @return Bks_Status indicator
 */
Bks_Status _bks_model_sql_favorite_products_get(Eina_List **list, const unsigned int limit); 

/**
 * @brief timestring of current local time in required format
 *
 * @return timestring of current local time in required format
 */
char* bks_model_current_time_string_get(void);

#endif
