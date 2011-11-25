#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

void
elm_list_clear_selection(Evas_Object *elm_list)
{
   Eina_List *iter, *list;
   Elm_List_Item *eli;

   list = (Eina_List*)elm_list_selected_items_get(elm_list);
   //deselect all items
   EINA_LIST_FOREACH(list, iter, eli)
     {
        if (elm_list_item_selected_get(eli))
          elm_list_item_selected_set(eli, EINA_FALSE);
     }
}


