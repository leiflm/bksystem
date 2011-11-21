#include <Evas.h>
#include <Elementary.h>

typedef struct _BKSystem_UI BKSystem_UI;
typedef struct _Elm_Naviframe_Page Elm_Naviframe_Page;

struct _Elm_Naviframe_Page {
      Evas_Object *content, *prev_btn, *next_btn;
      Elm_Object_Item *eoi;
};

struct _BKSystem_UI {
   Evas_Object *naviframe;
   Elm_Naviframe_Page productslist, accountslist;
};

static BKSystem_UI ui;

static void
clear_elm_list_selection(Evas_Object *elm_list)
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

static void
print_sale(void)
{
   Eina_List *selected_accounts = (Eina_List*)elm_list_selected_items_get(ui.accountslist.content);
   Eina_List *iter;
   Elm_List_Item *selected_product, *eli;
   char *account_name, *product_name;

   if (!(selected_product = elm_list_selected_item_get(ui.productslist.content)))
     return;

   if (!(product_name = (char*)elm_list_item_data_get(selected_product)))
     return;

   printf("%s was bought by:\n", product_name);
   // print the names of all selected accounts
   EINA_LIST_FOREACH(selected_accounts, iter, eli)
     {
        if (elm_list_item_selected_get(eli))
          {
             account_name = (char*)elm_list_item_data_get(eli);
             printf("\t%s\n", account_name);
          }
     }
}

static void
on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

static void
on_product_select(void *data, Evas_Object *obj, void *event_info)
{
   const char *name = (const char*)data;

   if (!data) return;
   printf("Product %s was selected\n", name);
   clear_elm_list_selection(ui.accountslist.content);
   elm_widget_disabled_set(ui.accountslist.next_btn, EINA_TRUE);
   elm_naviframe_item_promote(ui.accountslist.eoi);
}

static void
on_account_select(void *data, Evas_Object *obj, void *event_info)
{
   const char *name = (const char*)data;

   if (!data) return;
   printf("Account %s was selected\n", name);

   elm_widget_disabled_set(ui.accountslist.next_btn, EINA_FALSE);
}

static void
on_account_double_click(void *data, Evas_Object *obj, void *event_info)
{
   if (eina_list_count(elm_list_selected_items_get(ui.accountslist.content)) > 1)
         return;
   print_sale();
   clear_elm_list_selection(ui.productslist.content);
   elm_naviframe_item_promote(ui.productslist.eoi);
}

static void
on_accountslist_prev_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   clear_elm_list_selection(ui.productslist.content);
   elm_naviframe_item_promote(ui.productslist.eoi);
}

static void
on_accountslist_finish_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   print_sale();
   clear_elm_list_selection(ui.productslist.content);
   elm_naviframe_item_promote(ui.productslist.eoi);
}

static void
fill_productslist(Evas_Object *pl)
{
   Evas_Object *ic;
   Elm_List_Item *li;
   char buf[256];

#define BKS_NEW_PRODUCT_LI(name) \
   ic = elm_icon_add(pl); \
   snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get()); \
   elm_icon_file_set(ic, buf, NULL); \
   elm_icon_scale_set(ic, 1, 1); \
   li = elm_list_item_append(pl, name, ic, NULL, on_product_select, name)

   BKS_NEW_PRODUCT_LI("Bier");
   BKS_NEW_PRODUCT_LI("Wasser");
   BKS_NEW_PRODUCT_LI("Landjaeger");

#undef BKS_NEW_PRODUCT_LI
}

static void
fill_accountslist(Evas_Object *al)
{
   Evas_Object *ic;
   Elm_List_Item *li;
   char buf[256];

#define BKS_NEW_ACCOUNT_LI(name) \
   ic = elm_icon_add(al); \
   snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get()); \
   elm_icon_file_set(ic, buf, NULL); \
   elm_icon_scale_set(ic, 1, 1); \
   li = elm_list_item_append(al, name, ic, NULL, on_account_select, name)

   BKS_NEW_ACCOUNT_LI("Wauer, Matthias");
   BKS_NEW_ACCOUNT_LI("Middelschulte, Leif");
   BKS_NEW_ACCOUNT_LI("Kienzler, Domink");

#undef BKS_NEW_ACCOUNT_LI
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   // new window - do the usual and give it a name (hello) and title (Hello)
   win = elm_win_util_standard_add("bierkellersystem", "Bierkellersystem");
   // when the user clicks "close" on a window there is a request to delete
   evas_object_smart_callback_add(win, "delete,request", on_done, NULL);

   // add naviframe
   ui.naviframe = elm_naviframe_add(win);
   evas_object_show(ui.naviframe);
   // add object as a resize object for the window (controls window minimum
   // size as well as gets resized if window is resized)
   elm_win_resize_object_add(win, ui.naviframe);


   //create and fill productslist
   ui.productslist.content = elm_list_add(win);
   fill_productslist(ui.productslist.content);
   elm_list_go(ui.productslist.content);
   evas_object_show(ui.productslist.content);
   ui.productslist.eoi = elm_naviframe_item_push(ui.naviframe, "Produkte", NULL, NULL, ui.productslist.content, NULL);

   //create and fill accountslist
   ui.accountslist.content = elm_list_add(win);
   elm_list_multi_select_set(ui.accountslist.content, EINA_TRUE);
   fill_accountslist(ui.accountslist.content);
   elm_list_go(ui.accountslist.content);
   evas_object_show(ui.accountslist.content);
   // Add button to go back to productslist
   ui.accountslist.prev_btn = elm_button_add(ui.naviframe);
   elm_object_text_set(ui.accountslist.prev_btn, "Zurück");
   evas_object_smart_callback_add(ui.accountslist.prev_btn, "clicked", on_accountslist_prev_btn_click, NULL);
   // Add button to finish shopping
   ui.accountslist.next_btn = elm_button_add(ui.naviframe);
   elm_object_text_set(ui.accountslist.next_btn, "Fertig");
   evas_object_smart_callback_add(ui.accountslist.next_btn, "clicked", on_accountslist_finish_btn_click, NULL);

   // Add Accounts page to naviframe
   ui.accountslist.eoi = elm_naviframe_item_push(ui.naviframe, "Konten", ui.accountslist.prev_btn, ui.accountslist.next_btn, ui.accountslist.content, NULL);

   // Add callback for doubleclick action
   evas_object_smart_callback_add(ui.accountslist.content, "clicked,double", on_account_double_click, NULL);

   elm_naviframe_item_promote(ui.productslist.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui.naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(win, 640, 480);
   evas_object_show(win);

   // run the mainloop and process events and callbacks
   elm_run();

   evas_object_del(ui.productslist.content);
   evas_object_del(ui.accountslist.content);
   evas_object_del(ui.naviframe);

   return 0;
}
ELM_MAIN()
