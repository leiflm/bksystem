#ifndef __BKS_JOB_H__
#define __BKS_JOB_H__

#include "Bks_Status.h"
#include "Bks_Types.h"

enum _Bks_Job_Type { BKS_JOB_SALE,
                     BKS_JOB_PRODUCTS_ALPHA_GET,
                     BKS_JOB_PRODUCTS_FAVS_GET };

struct _Bks_Job {
     Bks_Job_Type type;
     Bks_Status status;
     void *data;
};

#endif
