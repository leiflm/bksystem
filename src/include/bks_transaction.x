#include "Bks_Transaction.h"

inline void transaction_append(Bks_Transaction_Type type)
{
   Eina_List *tl = NULL;
   Bks_Transaction *ta = NULL;

   eina_lock_take(&transaction_list);
   tl = ecore_thread_global_data_find(TRANSACTION_KEY);
   ta = malloc(sizeof(*ta));
   ta->type = type;
   tl = eina_list_append(tl, ta);
   ecore_thread_global_data_set(TRANSACTION_KEY, tl, _transaction_list_free_cb);
   eina_lock_release(&transaction_list);
}

inline Bks_Transaction *transactions_first_get(void)
{
   Eina_List *tl = NULL;
   Bks_Transaction *ta = NULL;

   eina_lock_take(&transaction_list);
   tl = ecore_thread_global_data_find(TRANSACTION_KEY);
   if (!tl)
     goto _first_get_return;
   ta = eina_list_data_get(ta);

_first_get_return:
   eina_lock_release(&transaction_list);
   return ta;
}
