#include "Bks_Types.h"
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Ui.h"

void bks_ui_controller_notification_set(const char* text, Elm_Popup_Orient orient, double timeout, Eina_Bool block_input)
{
   ecore_thread_main_loop_begin();
   elm_object_text_set(ui.notification.note, text);
   evas_object_show(ui.notification.note);
   elm_popup_timeout_set(ui.notification.note, timeout);
   elm_popup_orient_set(ui.notification.note, orient);
   elm_popup_allow_events_set(ui.notification.note, !block_input);
   ecore_thread_main_loop_end();
}

void _bks_ui_sale_notification_set(Bks_Model_Sale *sale) {
   char buf[4096];
   Bks_Model_User_Account *acc;
   Eina_Bool block_input = EINA_TRUE;
   double timeout = -1.0;
   Elm_Popup_Orient orient = ELM_POPUP_ORIENT_CENTER;
   size_t count = 0;

   if (sale->status == BKS_MODEL_SALE_DONE) {
      count += snprintf(buf, 256, "Kauf von %s: %.2f EUR wurde ", sale->productname, sale->price);
      // append every name in list
      EINA_LIST_FREE(sale->user_accounts, acc)
        {
           if (count < (sizeof(buf) - 32))
             count += snprintf((buf + count), (sizeof(buf) - count - 1), "%s %s, ", acc->firstname, acc->lastname);
        }
      //keep buffer overflows outside
      count = (count > (sizeof(buf) - 32)) ? (sizeof(buf) - 32) : count;
      //remove the last ", "
      buf[count - 2] = '\0';
      strcat(buf, " berechnet.");
      timeout = (2.0 + (double)eina_list_count(sale->user_accounts));
      orient = ELM_POPUP_ORIENT_BOTTOM_RIGHT;
      block_input = EINA_FALSE;
      sale->user_accounts = NULL;
   } else  {
      sprintf(buf, "ERROR: Verkauf konnte nicht durchgeführt werden!");
      block_input = EINA_TRUE;
   }

   bks_ui_controller_notification_set(buf, orient, timeout, block_input);
}

void _bks_ui_sale_notification_clicked(void *data UNUSED, Evas_Object *obj, void *event_info UNUSED)
{
   evas_object_hide(obj);
}
