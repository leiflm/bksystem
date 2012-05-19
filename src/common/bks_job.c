#include <stdlib.h>
#include "Bks_Job.h"

Bks_Job* bks_job_new(Bks_Job_Type type)
{
   Bks_Job *job = calloc(1, sizeof(Bks_Job));

   job->type = type;

   return job;
}

void bks_job_free(Bks_Job *job)
{
   free(job);
}
