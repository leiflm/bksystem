#include <ctype.h>
#include <Elementary.h>
#include "Elm_Utils.h"
#include "Bks_System.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"
#include "Bks_Ui_Private.h"

static void
_on_user_account_select(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   const Bks_Model_User_Account *acc = NULL;

   acc = (const Bks_Model_User_Account*)data;
   EINA_SAFETY_ON_NULL_RETURN(acc);
#ifdef DEBUG
   printf("Account %s, %s was selected\n", acc->lastname, acc->firstname);
#endif
   elm_object_disabled_set(ui.user_accounts.enp.next_btn, EINA_FALSE);
}

static void
_del_cb(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   Bks_Model_User_Account *acc = NULL;

   acc = (Bks_Model_User_Account*)data;
   EINA_SAFETY_ON_NULL_RETURN(acc);

   bks_model_user_account_free(acc);
}

static void
_on_index_select(void *data, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   Elm_Object_Item *li = (Elm_Object_Item*)data;

   if (!li)
       return;
   elm_list_item_show(li);
   // Use following line instead of the one above,
   // if system has enough power
   //elm_list_item_bring_in(li);
}

/*
static void
_on_user_account_double_click(void *data UNUSED, Evas_Object *obj UNUSED, void *event_info UNUSED)
{
   //Only react if only a single account is selected
   if (eina_list_count(elm_list_selected_items_get(ui.user_accounts.enp.content)) > 1)
     return;
   bks_controller_ui_sale_finish();
   products_page_reset();
   elm_naviframe_item_promote(ui.products.enp.eoi);
}
*/

void
user_accounts_list_set(Eina_List *user_accounts)
{
   Bks_Model_User_Account *acc;
   Elm_Object_Item *li;
   Evas_Object *ic = NULL;
   char buf[256], index_letter[2], uppered_letter = '0';
   const char *img_name = NULL;

   EINA_SAFETY_ON_NULL_RETURN(ui.user_accounts.list);
   EINA_SAFETY_ON_NULL_RETURN(user_accounts);

   index_letter[1] = '0';
   index_letter[1] = '\0';

   ecore_thread_main_loop_begin();
   elm_index_autohide_disabled_set(ui.user_accounts.index, EINA_FALSE);
   EINA_LIST_FREE(user_accounts, acc)
     {
        switch (acc->status)
          {
           case 1:
              img_name = "locked";
              break;
           default:
              img_name = NULL;
              break;
          }
        if (!img_name)
          ic = NULL;
        else
          {
             ic = elm_icon_add(ui.user_accounts.list);
             snprintf(buf, (sizeof(buf) - 1), "%s/images/accounts/%s.png", elm_app_data_dir_get(), img_name);
             elm_icon_standard_set(ic, buf);
             elm_image_resizable_set(ic, 0.0, 0.0);
#ifdef DEBUG
             printf("Trying to open %s as icon\n", buf);
#endif
          }
        snprintf(buf, (sizeof(buf) - 1), "%s, %s", acc->lastname, acc->firstname);
        li = elm_list_item_append(ui.user_accounts.list, buf, NULL, ic, _on_user_account_select, acc);
        elm_object_item_del_cb_set(li, _del_cb);
        if (acc->lastname)
          {
            uppered_letter = toupper(acc->lastname[0]);
            if (uppered_letter > index_letter[0])
              {
                index_letter[0] = uppered_letter;
                elm_index_item_append(ui.user_accounts.index, index_letter, _on_index_select, li);
              }
          }
     }
   elm_index_autohide_disabled_set(ui.user_accounts.index, EINA_TRUE);
   elm_list_go(ui.user_accounts.list);
   ecore_thread_main_loop_end();
}

// The function below is part of a workaround for an index bug
// where the index takes the entire canvas and thus blocks other elements
// such as buttons
static void
_adjust_index_size(void *data UNUSED, Evas *evas UNUSED, Evas_Object *obj, void *event_data UNUSED)
{
    Evas_Coord w = 0, h = 0;

#define INDEX_LETTER_WIDTH 200
    evas_object_geometry_get(obj, NULL, NULL, &w, &h);
    evas_object_resize(ui.user_accounts.index, INDEX_LETTER_WIDTH, h);
    evas_object_move(ui.user_accounts.index, (w - INDEX_LETTER_WIDTH), 55);
    evas_object_show(ui.user_accounts.index);
#undef INDEX_LETTER_WIDTH
}

Evas_Object *user_accounts_page_list_add(void)
{
   if (!ui.win) return NULL;

   ui.user_accounts.list = elm_list_add(ui.win);
   elm_list_multi_select_set(ui.user_accounts.list, EINA_TRUE);

   ui.user_accounts.index = elm_index_add(ui.win);
   EXPAND_AND_FILL(ui.user_accounts.index);
   evas_object_show(ui.user_accounts.index);
   // The callback added below is part of a workaround for an index bug
   // where the index takes the entire canvas and thus blocks other elements
   // such as buttons
   evas_object_event_callback_add(ui.user_accounts.list, EVAS_CALLBACK_RESIZE, _adjust_index_size, NULL);

   // Add callback for doubleclick action
   //evas_object_smart_callback_add(ui.user_accounts.enp.content, "clicked,double", _on_user_account_double_click, NULL);
   //evas_object_smart_callback_add(ui.user_accounts.list, "clicked,double", _on_user_account_double_click, NULL);

   return ui.user_accounts.list;
}
