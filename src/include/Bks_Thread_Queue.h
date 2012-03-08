#ifndef __BKS_THREAD_QUEUE_H__
#define __BKS_THREAD_QUEUE_H__

#include <Eina.h>
#include "Bks_Types.h"

enum _Bks_Thread_Type {
     BKS_THREAD_TYPE_NONE,
     BKS_THREAD_TYPE_USER_ACCOUNTS_GET,
     BKS_THREAD_TYPE_USER_ACCOUNTS_GET_FINISHED,
     BKS_THREAD_TYPE_PRODUCTS_GET,
     BKS_THREAD_TYPE_PRODUCTS_GET_FINISHED
};

struct _Bks_Thread_Queue_Element {
     Eina_Lock lock;
     double timestamp;
     Bks_Thread_Type type;
     void *data;
};


void bks_thread_queue_init(void);
void bks_thread_queue_shutdown(void);
Bks_Thread_Queue_Element *bks_thread_queue_append(const Bks_Thread_Type type, const double timestamp);
void bks_thread_queue_wait(Bks_Thread_Queue_Element *element);
void bks_thread_queue_wake_up_next(Bks_Thread_Queue_Element *current_element);
#endif
