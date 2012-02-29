#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

void
elm_list_selection_clear(Evas_Object *elm_list)
{
   const Eina_List *iter, *list;
   Elm_Object_Item *eoi;

   if (!elm_list) return;

   list = elm_list_selected_items_get(elm_list);
   //deselect all items
   EINA_LIST_FOREACH(list, iter, eoi)
     {
        if (elm_list_item_selected_get(eoi))
          elm_list_item_selected_set(eoi, EINA_FALSE);
     }
}

void
elm_gengrid_selection_clear(Evas_Object *gengrid)
{
   Elm_Object_Item *eoi;

   if (!gengrid) return;

   if ((eoi = elm_gengrid_selected_item_get(gengrid)))
     elm_gengrid_item_selected_set(eoi, EINA_FALSE);
}
