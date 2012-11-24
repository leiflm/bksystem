#!/bin/sh
# read backup data from files
#
# use export DB="path/to/database/name.sqlite

   echo ""
   echo "Executing script file:"
   echo " ${0}:"
if [ -z $BKS_BKP_DIR ]; then
   BKS_BKP_DIR="."
fi

cd $BKS_BKP_DIR
echo "cd ${BKS_BKP_DIR}"

# check variables
   if [ -z $DB ]; then
      DB="./bksystem.sqlite"
   fi
   if [ -e $DB ]; then
      echo "   File ${DB} already exists. aborting!"
      echo "   Use: export DB=\"path/to/database/name.sqlite\" to define a new name."
      exit 1
   fi

echo "schema reading.."
sqlite3 -batch $DB ".read ${BKS_BKP_DIR}/bksystem_scheme.sql"
echo "products reading..."
sqlite3 -batch $DB ".read products.sql"
echo "user_accounts reading...."
sqlite3 -batch $DB ".read user_accounts.sql"
echo "sales reading....."
sqlite3 -batch $DB ".read sales.sql"
echo "bill_dates reading....."
sqlite3 -batch $DB ".read bill_dates.sql"
echo "finished restore."

cd -
exit 0
