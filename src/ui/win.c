#include <Elementary.h>
#include <Evas.h>
#include "Bks_System.h"
#include "Bks_Ui_Private.h"

static void
win_on_done(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

Evas_Object *bks_ui_win_add(void)
{
   Evas_Object *win;

   win = elm_win_util_standard_add(MAIN_WINDOW_NAME, "Bierkellersystem");

   // when the user clicks "close" on a window there is a request to delete
   evas_object_smart_callback_add(win, "delete,request", win_on_done, NULL);

   return win;
}
