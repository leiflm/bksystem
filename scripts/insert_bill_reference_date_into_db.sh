#!/bin/sh
# script inserts current date and time into the database
# DB and date should be defined

   echo ""
   echo "Executing script file:"
   echo "${0}:"

# check variables
   if [ -z $DB ]; then
      DB="./bksystem.sqlite"
   fi

   if [ -z $BKS_DATE ]; then
      DATE=$(date +"%0Y-%0m-%0d_%0k:%0M:%0S")
   else
      DATE=$BKS_DATE
   fi
   # reformat date, replace underscore with space
   DATESQL="$(expr substr $DATE 1 10) $(expr substr $DATE 12 8)"
   echo "  Started inserting ${DATESQL} to DB"

   if ! [ -e $DB ]; then
      echo "ERROR:    Database missing or not found! Aborting." 1>&2
      exit 1
   fi

# insert current date to DB to trigger bill table generation
   STMT="INSERT INTO bill_dates (timestamp) VALUES ('${DATESQL}');"
   #for testing: STMT="INSERT INTO bill_dates (timestamp) VALUES ('2013-01-01 12:12:12');"
   # echo sqlite3 -column $DB $STMT
   sqlite3 -column "${DB}" "$STMT"
   REPLY=$?
   echo $REPLY > $ERRCODE
   exit $REPLY
