#ifndef __BKS_TYPES_H__
#define __BKS_TYPES_H__

#include "Bks_Controller.h"
#include "Bks_Ui.h"
#include "Bks_Model.h"
#include "Bks_Model_User_Account.h"
#include "Bks_Model_Product.h"
#include "Bks_Model_Sale.h"
#include "Bks_Status.h"

typedef struct _Bks_Controller Bks_Controller;
typedef struct _Bks_Ui Bks_Ui;
typedef struct _Bks_Model Bks_Model;
typedef struct _Bks_Model_User_Account Bks_Model_User_Account;
typedef struct _Bks_Model_Product Bks_Model_Product;
typedef struct _Bks_Model_Sale Bks_Model_Sale;
typedef struct _Bks_Transaction Bks_Transaction;
typedef enum _Bks_Status Bks_Status;
typedef enum _Bks_Job_Type Bks_Job_Type;
typedef struct _Bks_Job Bks_Job;
typedef enum _Bks_Thread_Type Bks_Thread_Type;
typedef struct _Bks_Thread_Queue_Element Bks_Thread_Queue_Element;
typedef enum _Bks_Model_Product_Status Bks_Model_Product_Status;
typedef enum _Bks_Model_User_Account_Status Bks_Model_User_Account_Status;

#endif
