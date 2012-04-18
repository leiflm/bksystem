#include <Evas.h>
#include "Bks_System.h"
#include "Bks_Controller.h"

static void _db_path_selected(void *data UNUSED, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *path = (Eina_Stringshare*)event_info;

   bks_controller_ui_db_path_set(path);
   evas_object_del(obj);
}

static void _bks_ui_controller_db_path_get(void *data UNUSED, Ecore_Thread *th UNUSED)
{
   Evas_Object *fs;

   fs = elm_fileselector_add(ui.win);

   evas_object_smart_callback_add(fs, "done", _db_path_selected, NULL);
}

void bks_ui_controller_db_path_get(void)
{
   ecore_thread_run(_bks_ui_controller_db_path_get, NULL, NULL, NULL);
}
