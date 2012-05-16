#include <Ecore.h>
#include "Bks_Events.h"

int BKS_EVENT_DB_NOT_FOUND;
int BKS_EVENT_DB_PATH_RETRIEVED;
int BKS_EVENT_ACCOUNTS_RELOAD;
int BKS_EVENT_ACCOUNTS_RELOADED;
int BKS_EVENT_PRODS_ALPHA_RELOAD;
int BKS_EVENT_PRODS_ALPHA_RELOADED;
int BKS_EVENT_PRODS_FAVS_RELOAD;
int BKS_EVENT_PRODS_FAVS_RELOADED;
int BKS_EVENT_SALE_TRY;
int BKS_EVENT_SALE_FINISHED;
int BKS_EVENT_UI_BLOCK_INPUT;
int BKS_EVENT_UI_UNBLOCK_INPUT;

void bks_events_init(void)
{
   ecore_init();
   BKS_EVENT_DB_NOT_FOUND = ecore_event_type_new();
   BKS_EVENT_DB_PATH_RETRIEVED = ecore_event_type_new();
   BKS_EVENT_ACCOUNTS_RELOAD = ecore_event_type_new();
   BKS_EVENT_ACCOUNTS_RELOADED = ecore_event_type_new();
   BKS_EVENT_PRODS_ALPHA_RELOAD = ecore_event_type_new();
   BKS_EVENT_PRODS_ALPHA_RELOADED = ecore_event_type_new();
   BKS_EVENT_PRODS_FAVS_RELOAD = ecore_event_type_new();
   BKS_EVENT_PRODS_FAVS_RELOADED = ecore_event_type_new();
   BKS_EVENT_SALE_TRY = ecore_event_type_new();
   BKS_EVENT_SALE_FINISHED = ecore_event_type_new();
   BKS_EVENT_UI_BLOCK_INPUT = ecore_event_type_new();
   BKS_EVENT_UI_UNBLOCK_INPUT = ecore_event_type_new();
}

void bks_events_shutdown(void)
{
   ecore_shutdown();
}
