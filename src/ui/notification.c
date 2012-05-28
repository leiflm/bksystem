#include "Bks_Types.h"
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Ui.h"


void _bks_ui_sale_notification_set(Bks_Model_Sale *sale) {

   Eina_List *l, *accs;
   int accs_count;
   int size, length;
   char *text;
   Bks_Model_User_Account *acc;

   if (sale->status == BKS_MODEL_SALE_DONE) {

      accs = sale->user_accounts;
      accs_count = eina_list_count(accs);
      // size for text + estimated size for user accounts
      size = 90 + accs_count * 30;
      text = calloc((size_t)(size + 1),1);
      length = snprintf(text, (size_t)size, "<align = left>Kauf von %s: %.2f EUR wurde ", sale->productname, sale->price);
      // append every name in list
      EINA_LIST_FOREACH(accs, l, acc) {
         if (size > length) {
            length += snprintf(text + length,(size_t)(size - length), "%s %s, ", acc->firstname, acc->lastname);
         }
      }
      if (size - length > 20) { // space for more than 20 char available
         length += snprintf(text + length - 2, (size_t)((size -length) - 2), " berechnet.</align>");
      } else { // not enough space, truncate names
         snprintf(text + size - 23, 23,"... berechnet.</align>");
      }

      elm_object_text_set(ui.notification.note, text);
      evas_object_show(ui.notification.note);
      elm_popup_timeout_set(ui.notification.note, 2.0 + (double)accs_count);
      free(text);
   } else  {
      elm_object_text_set(ui.notification.note, "ERROR: Verkauf konnte nicht durchgeführt werden!");
      evas_object_show(ui.notification.note);
      elm_popup_orient_set(ui.notification.note, ELM_POPUP_ORIENT_CENTER);
      elm_popup_allow_events_set(ui.notification.note, EINA_FALSE);
      elm_popup_timeout_set(ui.notification.note, -1.0);
   }
}
void _bks_ui_sale_notification_clicked(void *data UNUSED, Evas_Object *obj, void *event_info UNUSED) {

   evas_object_hide(obj);
   elm_popup_orient_set(ui.notification.note, ELM_POPUP_ORIENT_BOTTOM_RIGHT);
   elm_popup_allow_events_set(ui.notification.note, EINA_TRUE);
}
