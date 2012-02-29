#include <Elementary.h>
#include <Evas.h>

static void
win_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

Evas_Object *bks_ui_win_create(void)
{
   Evas_Object *win;

   win = elm_win_util_standard_add("bierkellersystem", "Bierkellersystem");

   // when the user clicks "close" on a window there is a request to delete
   evas_object_smart_callback_add(win, "delete,request", win_on_done, NULL);

   return win;
}
