#!/bin/sh
# 

echo ""
echo "Executing script file:"
echo "${0}:"

if [ -z $DB ]; then
   DB="./bksystem.sqlite"
fi

if [ -z $BKS_BKP_DIR ]; then
   BKS_BKP_DIR="."
fi

SCHEM="${BKS_BKP_DIR}/bksystem_schema1.sql"
USER="${BKS_BKP_DIR}/user_accounts.sql"
PROD="${BKS_BKP_DIR}/products.sql"
SALE="${BKS_BKP_DIR}/sales.sql"
BDATES="${BKS_BKP_DIR}/bill_dates.sql"

sqlite3 -batch $DB ".dump" > $SCHEM
if [ $? -eq 0 ]; then
   echo "   ${SCHEM} written"
fi
sqlite3 -batch $DB ".dump user_accounts" > $USER
if [ $? -eq 0 ]; then
   echo "   ${USER} written"
fi
sqlite3 -batch $DB ".dump products" > $PROD
if [ $? -eq 0 ]; then
   echo "   ${PROD} written"
fi
sqlite3 -batch $DB ".dump sales" > $SALE
if [ $? -eq 0 ]; then
   echo "   ${SALE} written"
fi
sqlite3 -batch $DB ".dump bills" > $BDATES
#sqlite3 -batch $DB "SELECT * FROM bill_dates WHERE bill_id != 0;" > $BDATES
if [ $? -eq 0 ]; then
   echo "   ${BDATES} written"
fi
