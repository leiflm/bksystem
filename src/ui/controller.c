#include <Eina.h>
#include <Ecore.h>
#include "Bks_Model_Sale.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

// API callbacks
void bks_ui_contoller_sale_finished_cb(Bks_Model_Sale *sale)
{
   EINA_SAFETY_ON_NULL_RETURN(sale);

   printf("Produktkauf %s durch %s, %s wurde in Rechnung gestellt.\n", sale->ean, sale->lastname, sale->firstname);

   bks_model_sale_free(sale);
}
