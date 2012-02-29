#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

void
elm_list_selection_clear(Evas_Object *elm_list)
{
   Eina_List *iter, *list;
   Elm_List_Item *eli;

   if (!elm_list) return;

   list = (Eina_List*)elm_list_selected_items_get(elm_list);
   //deselect all items
   EINA_LIST_FOREACH(list, iter, eli)
     {
        if (elm_list_item_selected_get(eli))
          elm_list_item_selected_set(eli, EINA_FALSE);
     }
}

void
elm_gengrid_selection_clear(Evas_Object *gengrid)
{
   Elm_Gengrid_Item *egi;

   if (!gengrid) return;

   if ((egi = elm_gengrid_selected_item_get(gengrid)))
     elm_gengrid_item_selected_set(egi, EINA_FALSE);
}
