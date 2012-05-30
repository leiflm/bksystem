#include <Eina.h>
#include <Ecore.h>
#include "Bks_Model_Sale.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

// API callbacks
void bks_ui_controller_sale_finished(Bks_Model_Sale *sale)
{
   ecore_main_loop_thread_safe_call_async(_bks_ui_sale_notification_set, sale);
   bks_model_sale_free(sale);
}
