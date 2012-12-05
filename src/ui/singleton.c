#include <Elementary.h>
#include <Evas.h>
#include "Bks_System.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

static void
_win_on_done(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

static void
_start_anyway(void *data, Evas_Object *obj, void *event_info)
{
    Evas_Object *win = (Evas_Object*)data;
    evas_object_hide(win);
    evas_object_del(win);
    bks_controller_ui_start_even_if_lock_exists();
}

static void
_cancel_start(void *data, Evas_Object *obj, void *event_info)
{
    elm_exit();
}

void _singleton_dialog_display(void)
{
   Evas_Object *win, *bg, *tb, *lb, *bt;

   win = elm_win_add(NULL, "Bierkellersystem - Laeuft bereits eine Instanz?", ELM_WIN_DIALOG_BASIC);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   EXPAND(bg);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   tb = elm_table_add(win);
   EXPAND(tb);
   elm_win_resize_object_add(win, tb);
   evas_object_show(tb);

   lb = elm_label_add(win);
   EXPAND_AND_FILL(lb);
   elm_object_text_set(lb, "<align=center>Kann es sein, dass bereits eine Instanz"
                           " des <b>Bierkeller Systems</b> </br>"
                           "laeuft bzw. gestartet wurde?</br>"
                           "</br>"
                           "Soll es trotzdem gestartet werden?</align>");
   elm_table_pack(tb, lb, 0, 0, 2, 1);
   evas_object_show(lb);

   bt = elm_button_add(win);
   FILL(bt);
   elm_object_text_set(bt, "Trotzdem starten?");
   evas_object_smart_callback_add(bt, "clicked", _start_anyway, win);
   elm_table_pack(tb, bt, 0, 1, 1, 1);
   evas_object_show(bt);

   bt = elm_button_add(win);
   FILL(bt);
   elm_object_text_set(bt, "Abbrechen");
   evas_object_smart_callback_add(bt, "clicked", _cancel_start, NULL);
   elm_table_pack(tb, bt, 1, 1, 1, 1);
   evas_object_show(bt);

   // when the user clicks "close" on a window there is a request to delete
   evas_object_smart_callback_add(win, "delete,request", _win_on_done, NULL);

   evas_object_resize(win, 320, 120);
   evas_object_show(win);
}
