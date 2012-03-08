#ifndef __BKS_SYSTEM_H__
#define __BKS_SYSTEM_H__
#include <stddef.h>
#include "Bks_Types.h"

#define UNUSED __attribute__((unused))

Bks_Model mdl;
Bks_Ui ui;
Bks_Controller ctrl;
Eina_Lock transaction_list;

#endif
