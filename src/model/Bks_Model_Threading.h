#ifndef __BKS_MODEL_THREADING_H__
#define __BKS_MODEL_THREADING_H__

#include <Ecore.h>

#include "Bks_Types.h"

/**
 * @brief Commits a sale to the database, starts therefore a new thread, 
 * @param struct sale with uid and EAN 
 *
 * @return Ecore_Thread indicator 
 */
Ecore_Thread *_bks_model_commit_sale(const Bks_Model_Sale *sale);

/**
 * @brief call that starts a thread to fetch the data into the model's data structure
 *
 * @description this is called once the ui is loaded it should set a lock until it loaded the data successfully
 * @return Thread handle immediately
 */
Ecore_Thread *_bks_model_products_get(const unsigned int limit);

/**
 * @brief call that starts a thread to fetch the data into the model's data structure
 *
 * @description this is called once the ui is loaded it should set a lock until it loaded the data successfully
 * @return Thread handle immediately
 */
Ecore_Thread *_bks_model_user_accounts_get(const unsigned int limit);

/**
 * @brief call that starts a thread to fetch the data into the model's data structure
 *
 * @description this is called once the ui is loaded it should set a lock until it loaded the data successfully
 * @return Thread handle immediately
 */
Ecore_Thread *_bks_model_data_get(void);

/**
 * @brief creates a new thread that creates a new database with final settlement for all and number of bought products
 *
 * @return success indicator of creating thread
 */
Ecore_Thread *_bks_model_create_bill_table(void);

#endif
