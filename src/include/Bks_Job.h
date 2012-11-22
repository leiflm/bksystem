#ifndef __BKS_JOB_H__
#define __BKS_JOB_H__

#include "Bks_Status.h"
#include "Bks_Types.h"

/*
 * Bks_Job is an entity used to prevent thread race conditions.
 * Imagine the following schedule of threaded events:
 *
 * Start sequence:
 * 1.) Clear user list
 * 2.) Get new user list
 * Actually executed sequence:
 * 1.) Got new user list
 * 2.) Cleared new user list
 *
 * It furthermore allows us to easily reschedule jobs if e.g. the database file
 * is not available.
 */
enum _Bks_Job_Type { BKS_JOB_SALE,
                     BKS_JOB_PRODUCTS_ALPHA_GET,
                     BKS_JOB_PRODUCTS_FAVS_GET,
                     BKS_JOB_USER_ACCOUNTS_FAVS_GET,
                     BKS_JOB_USER_ACCOUNTS_GET };

struct _Bks_Job {
     Bks_Job_Type type;
     Bks_Status status;
     void *data;
};

Bks_Job* bks_job_new(Bks_Job_Type type);
void bks_job_free(Bks_Job *job);

#endif
