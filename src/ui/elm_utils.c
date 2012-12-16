#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

void
elm_list_selection_clear(Evas_Object *elm_list)
{
   Eina_List *list, *iter, *lnext;
   Elm_Object_Item *eoi;

   if (!elm_list) return;

   list = elm_list_selected_items_get(elm_list);
   //deselect all items
   //EINA_SAFETY_ON_NULL_RETURN(list);

   EINA_LIST_FOREACH_SAFE(list, iter, lnext, eoi)
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
