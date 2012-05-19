//      Bks_Model_User_Account.h
//
//      Copyright 2011 Matthias Wauer <matthiaswauer@googlemail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef __BKS_MODEL_USER_ACCOUNT_H__
#define __BKS_MODEL_USER_ACCOUNT_H__

#include <sqlite3.h>
#include "Bks_Types.h"

    struct _Bks_Model_User_Account {
      int uid;
      char *firstname;
      char *lastname;
      Bks_Model_User_Account_Status status;
      struct {
         void *data;
         int size;
      } image;
    };

enum _Bks_Model_User_Account_Status { BKS_MODEL_USER_ACCOUNT_ACTIVE,
                                      BKS_MODEL_USER_ACCOUNT_LOCKED };

/**
 * @brief frees struct memory and NULLs
 *
 * 	frees memory occupied by struct Bks_Model_User_Account WARNING: Unused pointers must be NULL
 *
 * @param pointer to struct
 */
void bks_model_user_account_free(Bks_Model_User_Account *user_account_ptr);

#endif
