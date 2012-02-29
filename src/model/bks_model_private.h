#ifndef __BKS_MODEL_H__
#define __BKS_MODEL_H__

/**
 * @brief Commits a sale to the database
 *
 * @return Bks_Error indicator
 */
Bks_Error _bks_model_commit_sale(const Bks_Model_Sale *sale);

/**
 * @brief timestring of current local time in required
 *
 * @return timestring of current local time in required
 */

char* bks_model_current_time_string_get();

/**
 * @brief call that fetches the data into the model's
 *
 * @description this is called once the ui is loaded it should block until
 * is
 */
void bks_model_data_get(const Bks_Model *model);

/**
 * @brief creates a new database with final settlement for all and number of bought products
 *
 * @return success indicator
 */
Bks_Error _bks_model_create_bill_table();

#endif
