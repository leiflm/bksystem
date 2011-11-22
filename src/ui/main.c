#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

typedef struct _BKSystem_UI BKSystem_UI;
typedef struct _Elm_Naviframe_Page Elm_Naviframe_Page;
typedef struct _Product Product;

struct _Elm_Naviframe_Page {
      Evas_Object *content, *prev_btn, *next_btn;
      Elm_Object_Item *eoi;
};

struct _BKSystem_UI {
   Evas_Object *naviframe;
   struct {
        Elm_Naviframe_Page enp;
        Evas_Object *panes, *favs, *alpha, *list, *grid;
   } productslist;
   struct {
        Elm_Naviframe_Page enp;
        Evas_Object *list;
   } accountslist;
};

struct _Product {
     unsigned long long ean;
     char *name;
     double price;
     const unsigned char *image;
};

static BKSystem_UI ui;
static Elm_Gengrid_Item_Class gic;
static Eina_List *products;

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
   Eina_List *selected_accounts = (Eina_List*)elm_list_selected_items_get(ui.accountslist.enp.content);
   Eina_List *iter;
   Elm_List_Item *selected_product, *eli;
   char *account_name, *product_name;

   if (!(selected_product = elm_list_selected_item_get(ui.productslist.list)))
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
   clear_elm_list_selection(ui.accountslist.list);
   elm_widget_disabled_set(ui.accountslist.enp.next_btn, EINA_TRUE);
   elm_naviframe_item_promote(ui.accountslist.enp.eoi);
}

static void
on_account_select(void *data, Evas_Object *obj, void *event_info)
{
   const char *name = (const char*)data;

   if (!data) return;
   printf("Account %s was selected\n", name);

   elm_widget_disabled_set(ui.accountslist.enp.next_btn, EINA_FALSE);
}

static void
reset_productslist_page(void)
{
   Elm_List_Item *eli;
   Elm_Gengrid_Item *egi;

   if ((eli = elm_list_selected_item_get(ui.productslist.list)))
     elm_list_item_selected_set(eli, EINA_FALSE);
   if ((egi = elm_gengrid_selected_item_get(ui.productslist.grid)))
     elm_gen_item_selected_set(egi, EINA_FALSE);
}

static void
on_account_double_click(void *data, Evas_Object *obj, void *event_info)
{
   if (eina_list_count(elm_list_selected_items_get(ui.accountslist.enp.content)) > 1)
         return;
   print_sale();
   reset_productslist_page();
   elm_naviframe_item_promote(ui.productslist.enp.eoi);
}

static void
on_accountslist_prev_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   reset_productslist_page();
   elm_naviframe_item_promote(ui.productslist.enp.eoi);
}

static void
on_accountslist_finish_btn_click(void *data, Evas_Object *obj, void *event_info)
{
   print_sale();
   clear_elm_list_selection(ui.productslist.list);
   elm_naviframe_item_promote(ui.productslist.enp.eoi);
}

static void
products_create(void)
{
   Product *product = NULL;
#define BKS_NEW_PRODUCT(product_ean, product_name, product_price) \
   product = calloc(1, sizeof(Product)); \
   product->ean = product_ean; \
   product->name = product_name; \
   product->price = product_price; \
   products = eina_list_append(products, product)

   BKS_NEW_PRODUCT(1, "Bier", 0.8);
   BKS_NEW_PRODUCT(2, "Cola", 0.55);
   BKS_NEW_PRODUCT(3, "Orangina", 1.2);

#undef BKS_NEW_PRODUCT
}

static void
productslist_fill(void)
{
   Evas_Object *ic;
   Eina_List *l;
   Elm_List_Item *li;
   Product *product;
   char buf[256];

   EINA_LIST_FOREACH(products, l, product)
     {
        ic = elm_icon_add(ui.productslist.list);
        snprintf(buf, sizeof(buf), "%s/%s.png", elm_app_data_dir_get(), product->name);
        elm_icon_file_set(ic, buf, NULL);
        elm_icon_scale_set(ic, 1, 1);
        li = elm_list_item_append(ui.productslist.list, product->name, ic, NULL, on_product_select, product->name);
     }

   printf(elm_app_data_dir_get());
}

static void
accountslist_fill(void)
{
   Evas_Object *ic;
   Elm_List_Item *li;
   char buf[256];

#define BKS_NEW_ACCOUNT_LI(name) \
   ic = elm_icon_add(ui.accountslist.list); \
   snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get()); \
   elm_icon_file_set(ic, buf, NULL); \
   elm_icon_scale_set(ic, 1, 1); \
   li = elm_list_item_append(ui.accountslist.list, name, ic, NULL, on_account_select, name)

   BKS_NEW_ACCOUNT_LI("Hauser, Kaspar");
   BKS_NEW_ACCOUNT_LI("Musterfrau, Petra");
   BKS_NEW_ACCOUNT_LI("Mustermann, Max");

#undef BKS_NEW_ACCOUNT_LI
}

static void
grid_selected(void *data, Evas_Object *obj, void *event_info)
{
   printf("Selected: %p\n", event_info);
}

static void
grid_longpress(void *data, Evas_Object *obj, void *event_info)
{
   printf("longpress %p\n", event_info);
}

char *
grid_label_get(void *data, Evas_Object *obj, const char *part)
{
   const Product *product = (Product*)data;
   char buf[256];

   if (!product) return NULL;

   snprintf(buf, sizeof(buf), "%s, %.2fEuro", product->name, product->price);
   return strdup(buf);
}

Evas_Object *
grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Product *product = (Product*)data;
   char buf[256];

   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *icon = elm_bg_add(obj);
        snprintf(buf, sizeof(buf), "/home/leif/Dokumente/source/bksystem/data/images/%llu.jpg", product->ean);
        elm_bg_file_set(icon, buf, NULL);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(icon);
        return icon;
     }
   /*
   else if (!strcmp(part, "elm.swallow.end"))
     {
	Evas_Object *ck;
	ck = elm_check_add(obj);
	evas_object_propagate_events_set(ck, 0);
	elm_check_state_set(ck, ti->onoff);
	evas_object_smart_callback_add(ck, "changed", grid_item_check_changed, data);
	evas_object_show(ck);
	return ck;
     }
   */
   return NULL;
}

Eina_Bool
grid_state_get(void *data, Evas_Object *obj, const char *part)
{
   return EINA_FALSE;
}

void
grid_del(void *data, Evas_Object *obj)
{
}

/*
static void
grid_item_check_changed(void *data, Evas_Object *obj, void *event_info)
{
   Testitem *ti = data;
   ti->onoff = elm_check_state_get(obj);
   printf("item %p onoff = %i\n", ti, ti->onoff);
}
*/

void productslist_favs_add(Evas_Object *parent)
{
   Eina_List *iter;
   Product *product;
   Elm_Gengrid_Item *li;

   ui.productslist.grid = elm_gengrid_add(parent);
   elm_gengrid_item_size_set(ui.productslist.grid, 150, 150);
   evas_object_size_hint_weight_set(ui.productslist.grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   gic.item_style = "default";
   gic.func.label_get = grid_label_get;
   gic.func.content_get = grid_content_get;
   gic.func.state_get = grid_state_get;
   gic.func.del = grid_del;


   EINA_LIST_FOREACH(products, iter, product)
      li = elm_gengrid_item_append(ui.productslist.grid, &gic, product, grid_selected, NULL);

   ui.productslist.favs = ui.productslist.grid;
}

void productslist_alpha_add(Evas_Object *parent)
{
   ui.productslist.list = elm_list_add(parent);
   productslist_fill();
   elm_list_go(ui.productslist.list);
   evas_object_show(ui.productslist.list);
   ui.productslist.alpha = ui.productslist.list;
}

void productslist_add(Evas_Object *parent)
{
   ui.productslist.panes = elm_panes_add(parent);
   elm_panes_horizontal_set(ui.productslist.panes, EINA_TRUE);

   productslist_favs_add(parent);
   elm_object_part_content_set(ui.productslist.panes, "left", ui.productslist.favs);
   productslist_alpha_add(parent);
   elm_object_part_content_set(ui.productslist.panes, "right", ui.productslist.alpha);

   ui.productslist.enp.content = ui.productslist.panes;
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
   products_create();
   productslist_add(win);
   ui.productslist.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Produkte", NULL, NULL, ui.productslist.enp.content, NULL);

   //create and fill accountslist
   ui.accountslist.list = elm_list_add(win);
   ui.accountslist.enp.content = ui.accountslist.list;
   elm_list_multi_select_set(ui.accountslist.list, EINA_TRUE);
   accountslist_fill();
   elm_list_go(ui.accountslist.enp.content);
   evas_object_show(ui.accountslist.enp.content);
   // Add button to go back to productslist
   ui.accountslist.enp.prev_btn = elm_button_add(ui.naviframe);
   elm_object_text_set(ui.accountslist.enp.prev_btn, "Zurück");
   evas_object_smart_callback_add(ui.accountslist.enp.prev_btn, "clicked", on_accountslist_prev_btn_click, NULL);
   // Add button to finish shopping
   ui.accountslist.enp.next_btn = elm_button_add(ui.naviframe);
   elm_object_text_set(ui.accountslist.enp.next_btn, "Fertig");
   evas_object_smart_callback_add(ui.accountslist.enp.next_btn, "clicked", on_accountslist_finish_btn_click, NULL);

   // Add Accounts page to naviframe
   ui.accountslist.enp.eoi = elm_naviframe_item_push(ui.naviframe, "Konten", ui.accountslist.enp.prev_btn, ui.accountslist.enp.next_btn, ui.accountslist.enp.content, NULL);

   // Add callback for doubleclick action
   evas_object_smart_callback_add(ui.accountslist.enp.content, "clicked,double", on_account_double_click, NULL);

   elm_naviframe_item_promote(ui.productslist.enp.eoi);
   elm_naviframe_content_preserve_on_pop_set(ui.naviframe, EINA_TRUE);

   // now we are done, show the window
   evas_object_resize(win, 640, 480);
   evas_object_show(win);

   // run the mainloop and process events and callbacks
   elm_run();

   evas_object_del(ui.productslist.enp.content);
   evas_object_del(ui.accountslist.enp.content);
   evas_object_del(ui.naviframe);

   return 0;
}
ELM_MAIN()
