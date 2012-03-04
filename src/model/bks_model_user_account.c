//      bks_model_user_account.c
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

#include <stdlib.h>

#include "Bks_Types.h"
#include "Bks_Model_User_Account.h"


void bks_model_user_account_free(Bks_Model_User_Account *account_ptr) {
     if (!account_ptr)
       return;
     free(account_ptr->firstname);
     free(account_ptr->lastname);
     free(account_ptr->image.data);
     free(account_ptr);
}
