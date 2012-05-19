/**
 * example usage:
 * Eina_List *l = users_get();
 * 
 * Eina_Bool _my_cb(void *data, int type, void *ev)
 * {
 *    Eina_List *l = (Eina_List*)ev;
 *    //code that frees list l...
 *    return ECORE_EVENT_RENEW; // keep the events coming!
 * }
 *
 * ecore_event_add(BKS_ACCOUNTS_RELOADED, l, _my_cb, NULL);
 */


/**
 * Emitters: model
 * Listeners: controller
 */
extern int BKS_EVENT_DB_NOT_FOUND;

/**
 * Emitters: ui
 * Listeners: contoller
 * Event structure *ev:
 * const char *path
 */
extern int BKS_EVENT_DB_PATH_RETRIEVED;

extern int BKS_EVENT_ACCOUNTS_RELOAD;
/**
 * Event structure *ev:
 * Eina_List *list of Bks_Model_User_Account*
 */
extern int BKS_EVENT_ACCOUNTS_RELOADED;

extern int BKS_EVENT_PRODS_ALPHA_RELOAD;
/**
 * Event structure *ev:
 * Eina_List *list of Bks_Model_Product*
 */
extern int BKS_EVENT_PRODS_ALPHA_RELOADED;

extern int BKS_EVENT_PRODS_FAVS_RELOAD;
/**
 * Event structure *ev:
 * Eina_List *list of Bks_Model_Product*
 */
extern int BKS_EVENT_PRODS_FAVS_RELOADED;

/**
 * Event structure *ev:
 * Bks_Sale *sale
 */
extern int BKS_EVENT_COMMIT_SALE;

/**
 * Event structure *ev:
 * Bks_Sale *reason
 */
extern int BKS_EVENT_COMMIT_SALE_FAILED;

/**
 * Event structure *ev:
 * const char *reason
 */
extern int BKS_EVENT_UI_LOCK;
extern int BKS_EVENT_UI_UNLOCK;

/**
 * Event structure *ev:
 * Bks_Status error
 */
extern int BKS_EVENT_UI_ERROR;


void bks_events_init(void);
void bks_events_shutdown(void);
