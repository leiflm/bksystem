#include "Bks_Thread_Queue.h"
#include <assert.h>

struct {
     Eina_List *queue;
     Eina_Lock lock;
} thread_queue;

static void _free_queue_element(Bks_Thread_Queue_Element *tqe);
static int _queue_sort_cb(const void *data1, const void *data2);

void bks_thread_queue_init(void)
{
   eina_lock_new(&thread_queue.lock);
   thread_queue.queue = NULL;
}

void bks_thread_queue_shutdown(void)
{
   Bks_Thread_Queue_Element *tqe = NULL;

   eina_lock_take(&thread_queue.lock);
   EINA_LIST_FREE(thread_queue.queue, tqe)
     {
        _free_queue_element(tqe);
     }

   eina_lock_free(&thread_queue.lock);
}

Bks_Thread_Queue_Element *bks_thread_queue_append(const Bks_Thread_Type type, const double timestamp)
{
   Bks_Thread_Queue_Element *tqe = NULL;

   eina_lock_take(&thread_queue.lock);
   tqe = malloc(sizeof(*tqe));
   tqe->timestamp = timestamp;
   tqe->type = type;
   eina_lock_new(&tqe->lock);
   eina_lock_take(&tqe->lock);
   thread_queue.queue = eina_list_sorted_insert(thread_queue.queue, _queue_sort_cb, tqe);
   eina_lock_release(&thread_queue.lock);

   return tqe;
}

void bks_thread_queue_wait(Bks_Thread_Queue_Element *element)
{
   Bks_Thread_Queue_Element *tqe = NULL;

//_th_equal_check:
   //Maybe it's the first element right away
   eina_lock_take(&thread_queue.lock);
   tqe = eina_list_data_get(thread_queue.queue);
   if (tqe == element)
     {
        eina_lock_release(&thread_queue.lock);
        return;
     }
   //release the lock so other threads (read "the one whose turn it is") can aquire it
   eina_lock_release(&thread_queue.lock);

   //or we need to sleep until it's the right time
   eina_lock_take(&element->lock);
//   safety check:
//   goto _th_equal_check;
}

void bks_thread_queue_wake_up_next(Bks_Thread_Queue_Element *element)
{
   Bks_Thread_Queue_Element *tqe = NULL;

   eina_lock_take(&thread_queue.lock);
   tqe = eina_list_data_get(thread_queue.queue);
   //verify that the first list element is what it has to be.
   assert(tqe == element);
   //remove ourself from the list
   thread_queue.queue = eina_list_remove_list(thread_queue.queue, thread_queue.queue);
   _free_queue_element(tqe);
   //wakeup next thread
   if ((tqe = eina_list_data_get(thread_queue.queue)))
     {
        eina_lock_release(&tqe->lock);
     }
   eina_lock_release(&thread_queue.lock);
}

// Helper functions:
int _queue_sort_cb(const void *data1, const void *data2)
{
   const Bks_Thread_Queue_Element *tqe1 = (Bks_Thread_Queue_Element*)data1;
   const Bks_Thread_Queue_Element *tqe2 = (Bks_Thread_Queue_Element*)data2;

   EINA_SAFETY_ON_NULL_RETURN_VAL(tqe1, 1);
   EINA_SAFETY_ON_NULL_RETURN_VAL(tqe2, -1);

   return (tqe2->timestamp - tqe1->timestamp);
}

void _free_queue_element(Bks_Thread_Queue_Element *tqe)
{
   eina_lock_free(&tqe->lock);
   free(tqe);
}
