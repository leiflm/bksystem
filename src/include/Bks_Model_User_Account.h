//      user_account.h
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

typedef struct _Bks_Model_User_Account Bks_Model_User_Account;

    struct _Bks_Model_User_Account {
        sqlite3_uint64 uid;
        char *firstname;
        char *lastname;
        sqlite3_uint64 status;
        void *image;
    }; 
    
#endif
