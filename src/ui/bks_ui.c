#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "Bks_Types.h"
#include "Bks_Model_Sale.h"
#include "Bks_System.h"
#include "Bks_Controller.h"
#include "Bks_Model.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

void bks_ui_init(void)
{
   // initialize Elementary
   elm_init(0, NULL);

   //Add mutexes
   eina_lock_new(&ui.products.lock);
   eina_lock_new(&ui.user_accounts.lock);

   // new window - do the usual and give it a name and title
   ui.win = bks_ui_win_add();

   // add naviframe
   ui.naviframe = elm_naviframe_add(ui.win);
   evas_object_show(ui.naviframe);
   // add object as a resize object for the window (controls window minimum
   // size as well as gets resized if window is resized)
   elm_win_resize_object_add(ui.win, ui.naviframe);

   //create and fill products
   ui.products.enp.content = products_page_add();
   ui.products.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Produkte", NULL, NULL, ui.products.enp.content, NULL);

   //create, fill and add user_accounts ui elements
   ui.user_accounts.enp.content = user_accounts_page_add();
   ui.user_accounts.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Konten", ui.user_accounts.enp.prev_btn, ui.user_accounts.enp.next_btn, ui.user_accounts.enp.content, NULL);

   elm_naviframe_item_promote(ui.products.enp.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui.naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(ui.win, 640, 480);
   evas_object_show(ui.win);
}

void bks_ui_shutdown(void)
{
   evas_object_del(ui.products.enp.content);
   ui.products.enp.content = NULL;
   evas_object_del(ui.user_accounts.enp.content);
   ui.user_accounts.enp.content = NULL;
   evas_object_del(ui.naviframe);
   ui.naviframe = NULL;
   evas_object_del(ui.win);
   ui.win = NULL;

   //Free mutexes
   eina_lock_free(&ui.products.lock);
   eina_lock_free(&ui.user_accounts.lock);

   elm_shutdown();
}
