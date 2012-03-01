#include <Eina.h>
#include <Ecore.h>
#include "Bks_Model_Sale.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

/*
 * @brief sale was finished.
 */
void _sale_commit_finished_cb(void *data, Ecore_Thread *th)
{
   Bks_Model_Sale *sale = (Bks_Model_Sale*)data;

   EINA_SAFETY_ON_NULL_RETURN(sale);

   printf("Produktkauf %s durch %s, %s wurde in Rechnung gestellt.\n", sale->ean, sale->lastname, sale->firstname);

   bks_model_sale_free(sale);
}

// API callbacks
void bks_ui_contoller_sale_finished_cb(Bks_Model_Sale *sale)
{
   EINA_SAFETY_ON_NULL_RETURN(sale);

   ecore_thread_run(_sale_commit_finished_cb, NULL, NULL, (void*)sale);
}
