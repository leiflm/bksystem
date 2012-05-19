#ifndef __BKS_STATUS_H__
#define __BKS_STATUS_H__

enum _Bks_Status { BKS_MODEL_USERACCOUNTS_GET,
                  BKS_MODEL_PRODUCTS_GET,
                  BKS_MODEL_BILL_GET,
                  BKS_CONTROLLER_SALE,
                  BKS_MODEL_SALE_UNFINISHED,
                  BKS_MODEL_SALE_DONE,
                  BKS_MODEL_SALE_ERROR,
                  BKS_MODEL_THREAD_ERROR,
                  BKS_MODEL_SQLITE_ERROR,
                  BKS_MODEL_SQLITE_OPEN_ERROR,
                  BKS_MODEL_SQLITE_WRITE_ERROR,
                  BKS_MODEL_SQLITE_DATABASE_LOCKED,
                  BKS_MODEL_SQLITE_ROW_MISSING,
                  BKS_MODEL_SQLITE_OK };

extern const char *BKS_STATUS_STRINGS[];

#endif
