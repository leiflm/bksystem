#!/bin/sh
# This script is intended to generate a bill from bksystem.sqlite database, export to csv.
# 3 tables:
# Account balance list
# Consumption of products list
# Consumption of each user list
# Steps:
# get current date and time
# sqlite querry and export tables as .csv


   echo ""
   echo "Executing script file:"
   echo "${0}:"
   
# DB and date should be defined, but if not:
   if [ -z $DB ]; then
      DB="./bksystem.sqlite"
   fi
   if [ -z $BKS_DATE ]; then
      BKS_DATE=$(date +"%0Y-%0m-%0d_%0k:%0M:%0S")
   fi
   # here go the exported files, should be defined, if not:
   if [ -z $BKS_BILL_DIR ]; then
      BKS_BILL_DIR="."
   fi

   if ! [ -e $DB ]; then
      echo "   Database missing or not found!"
      exit 1
   fi
   echo "   Started exporting to files"

#filenames + paths
   FILENAMEA="Kontostand"
   FILENAMEC="Verbrauch"
   FILENAMEL="Konsumliste"
   CSVFA="${BKS_BILL_DIR}/${FILENAMEA}_${BKS_DATE}.csv"
   CSVFC="${BKS_BILL_DIR}/${FILENAMEC}_${BKS_DATE}.csv"
   CSVFL="${BKS_BILL_DIR}/${FILENAMEL}_${BKS_DATE}.csv"
   #CSVFA="${BKS_BILL_DIR}/${FILENAMEA}.csv"
   #CSVFC="${BKS_BILL_DIR}/${FILENAMEC}.csv"
   #CSVFL="${BKS_BILL_DIR}/${FILENAMEL}.csv"

# export bill and consumption table as csv
   sqlite3 -csv -header $DB "SELECT user_account_id AS 'id', lastname, firstname, sum FROM previous_account_balances;" > $CSVFA
   if [ $? -eq 0 ]; then
      echo "   ${CSVFA} written."
   else
      echo "   ${CSVFA} writing failed"
   fi

   sqlite3 -csv -header $DB "SELECT product_id AS 'id', name, price, count, sum FROM previous_consumptions;" > $CSVFC
   if [ $? -eq 0 ]; then
      echo "   ${CSVFC} written."
   else
      echo "   ${CSVFC} writing failed"
   fi

   sqlite3 -csv -header $DB "SELECT id,user_account_id,lastname,firstname,product_id,ean,name,price,count,sum FROM previous_account_purchases;" > $CSVFL
   if [ $? -eq 0 ]; then
      echo "   ${CSVFL} written."
   else
      echo "   ${CSVFL} writing failed"
   fi
