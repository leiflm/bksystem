#include "Bks_Types.h"
#include <Evas.h>
#include <Elementary.h>
#include "Bks_System.h"
#include "Bks_Ui.h"


Evas_Object *sale_popup;


void bks_ui_sale_notification_set(Bks_Model_Sale *sale) {

   Eina_List *l = (Eina_List*)sale->user_accounts;
   Bks_Model_User_Account *acc = (Bks_Model_User_Account*)l->data;
   //EINA_LIST_FOREACH(accs, l, acc)
   //   printf("Produktkauf %llu durch %s, %s wurde in Rechnung gestellt.\n", sale->EAN);
   char *text = calloc(256,1);
   snprintf(text, 256, "<align = center>Kauf von %s (%.2fEUR) wurde %s %s berechnet.</align>", sale->productname, sale->price, acc->firstname, acc->lastname);
   //elm_object_part_text_unset(ui.notification.note);
   elm_object_text_set(ui.notification.content, text);
   elm_object_content_set(ui.notification.note, ui.notification.content);
      elm_popup_timeout_set(ui.notification.note, 3.0);
   evas_object_show(ui.notification.note);

}

void _bks_ui_sale_notification_time_out(void *data, Evas_Object *obj, void *event_info) {

   evas_object_hide(obj);
   fprintf(stderr, "notification now should hide");
   }

void _bks_ui_sale_notification_clicked(void *data, Evas_Object *obj, void *event_info) {

   evas_object_hide(obj);
   fprintf(stderr, "notification now should hide");
   }
