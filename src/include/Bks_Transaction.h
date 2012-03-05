#ifndef __BKS_TRANSACTION_H__
#define __BKS_TRANSACTION_H__

#include "Bks_Types.h"

#define TRANSACTION_KEY "transaction_list"

enum _Bks_Transaction_Type {
     BKS_TRANSACTION_NONE,
     BKS_TRANSACTION_USER_ACCOUNTS_GET,
     BKS_TRANSACTION_USER_ACCOUNTS_GET_FINISHED
     BKS_TRANSACTION_PRODUCTS_GET,
     BKS_TRANSACTION_PRODUCTS_GET_FINISHED
};

struct _Bks_Transaction {
     Bks_Transaction_Type type;
};

void _transaction_list_free_cb(void *data)
{
   Eina_List *tl = (Eina_List*)data;
   Bks_Transaction *ta = NULL;
   EINA_LIST_FREE(tl, ta)
     {
        free(ta);
     }
}
#endif
