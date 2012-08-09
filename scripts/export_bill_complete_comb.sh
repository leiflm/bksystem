#!/bin/sh
# This script is intended to export a bill from bksystem.sqlite database to csv. It will generate a user(rows) X products(colums) pivot table, with count as data.
# same as export_bill_complete_sep.sh but combined first and lastname
#
# Steps:
#
# get current date and time
# export tables user, product and bill as .csv
# generate sqlfile with statements for:
#   CREATE TABLE with products as columns
#   INSERT user rows (each user)
#   UPDATE user rows with consumption (each user and product)
# query table and add 2 first and last lines
# export table as .csv
# cleanup af tmp files

   echo ""
   echo "Executing script file:"
   echo "${0}:"
   echo " Started exporting complete bill..."

# DB and date should be defined, but if not:
   if [ -z $DB ]; then
      DB="./bksystem.sqlite"
   fi
   if [ -z $BKS_DATE ]; then
      DATE=$(date +"%0Y-%0m-%0d_%0k:%0M:%0S")
   else
      DATE=$BKS_DATE
   fi
   DATEFILE="${DATE}"
   #DATEPRINT="${DATE:8:2}.${DATE:5:2}.${DATE:0:4} ${DATE:11:8}"

#filenames + paths for the exported files, should be defined, if not:
   # exported will be written here
   if [ -z $BKS_BILL_DIR ]; then
      BKS_BILL_DIR="."
   fi
   FILENAME="Abrechnung"
   CSVF="${BKS_BILL_DIR}/${FILENAME}_${DATEFILE}_table.csv"
   #CSVF="${BKS_BILL_DIR}/${FILENAME}_table.csv"
   echo " Filename:"
   echo "   ${CSVF}"


   # set tmp directory
   if [ -z $BKS_TMP_DIR ]; then
      BKS_TMP_DIR="."
   fi
   BKS_TMP_SUB_DIR="${BKS_TMP_DIR}/comb"
   if ! [ -d $BKS_TMP_SUB_DIR ]; then
      mkdir $BKS_TMP_SUB_DIR
   fi
   # tmp files
      # For CEATE TABLE stmt
      CSV_PROD_ID="${BKS_TMP_SUB_DIR}/product_ids.csv"
      PROD_LIST="${BKS_TMP_SUB_DIR}/product_list_sql.csv"
      # For INSERT user
      CSV_USER="${BKS_TMP_SUB_DIR}/user.csv"
      # For UPDATE user
      CSV_BILL="${BKS_TMP_SUB_DIR}/bill.csv"
      # completed sql batch querry to generate and fill table
      SQL_Q="${BKS_TMP_SUB_DIR}/create_querry.sql"
      # for header lines
      CSV_PROD_NAME="${BKS_TMP_SUB_DIR}/product_names.csv"
      CSV_PROD_PRICE="${BKS_TMP_SUB_DIR}/product_prices.csv"
      CSV_PROD_NAME_ID="${BKS_TMP_SUB_DIR}/product_name_prices.csv"
      # for tail line
      SUM_ROW="${BKS_TMP_SUB_DIR}/sum_row.csv"

   # ensure tmp files be empty
   rm $CSV_PROD_ID $PROD_LIST $CSV_USER $CSV_BILL $SQL_Q $CSV_PROD_NAME $CSV_PROD_PRICE $CSV_PROD_NAME_ID $SUM_ROW >/dev/null 2>&1

# generate SQL batch file
   echo " Generating batch file:"
   echo "   ${SQL_Q}"

   # get product list, user list and user/product accumulated list, these tables are

   # get product list
   sqlite3 -csv "${DB}" "SELECT EAN, price FROM previous_consumption;" > $CSV_PROD_ID
   # test for data existence
   if ! [ -s $CSV_PROD_ID ]; then
      echo "WARNING: Seems to be nothing sold. Aborting generation" 1>&2
      exit 0;
   fi

   # Product List: Quote all ID numbers and append type definiton, then remove linebreaks
    sed 's/\([0-9][0-9]*,[0-9]*.[0-9]*\)/\"\1EUR\" INTEGER  DEFAULT 0,/g' $CSV_PROD_ID | sed ':a;N;$!ba;s/\n//g' >$PROD_LIST
   #sed 's/\([0-9][0-9]*\)/\"\1\" INTEGER,/g' $CSV_PROD_ID | sed ':a;N;$!ba;s/\n//g' >$PROD_LIST

   # create file with all sql commands
   # creat temporary table uid, names, productlist, sum
      echo "   insert CREATE TABLE ..."
      echo "CREATE TEMP TABLE \"bill_comp\" ( UID INTEGER PRIMARY KEY, name VARCHAR, $(cat ${PROD_LIST}) sum FLOAT);" > $SQL_Q


   # loop on user-list
      echo "   insert INSERT users..."
      # get user list
      sqlite3 -csv "${DB}" "SELECT userid, lastname, firstname, sum FROM previous_account_balance;" > $CSV_USER
      # remove quotes
      sed 's/\"//g' $CSV_USER |
      # parse user info, generate INSERT statement for each user
      # insert lastname, firstname into one field
      while IFS=, read -r USERID LNAME FNAME SUM ; do
         echo "INSERT INTO \"bill_comp\" ( UID, name,  sum) VALUES (${USERID}, \"${LNAME}, ${FNAME}\", ${SUM});" >> $SQL_Q
      done

   # loop on complete_bill-list to write update statement for each product, only fields that are not = 0 are in this list
      echo "   insert UPDATE users.."
      # get user/product accumulated list
      sqlite3 -csv "${DB}" "SELECT userid, EAN, price, count FROM previous_account_list;" > $CSV_BILL
      while IFS=, read -r CUR_UID CUR_PROD_ID CUR_PRICE CUR_PROD_VAL ; do
         echo "UPDATE \"bill_comp\" SET \"${CUR_PROD_ID},${CUR_PRICE}EUR\"=${CUR_PROD_VAL} WHERE UID = ${CUR_UID};"  >> $SQL_Q
      done < $CSV_BILL
      echo " batch file completed..."
      # cat $SQL_Q

# create 2 header lines/ 1.replace EANs with name 2. with price
   echo " creating table header..."
   # get all sold product with name and price, order by ID
   sqlite3 -csv -separator '#' $DB "SELECT products.name, previous_consumption.price FROM previous_consumption, products WHERE products.EAN=previous_consumption.EAN ORDER by products.EAN" > $CSV_PROD_NAME_ID
   # parse list, remove quotes around name, new quotes around combined name+price, append to new list

   while IFS='#' read -r CUR_PROD_NAME CUR_PROD_PRICE ; do
      echo "\"$(echo ${CUR_PROD_NAME} | sed 's/\"//g' )\","  >> $CSV_PROD_NAME
      echo "${CUR_PROD_PRICE},"  >> $CSV_PROD_PRICE
   done < $CSV_PROD_NAME_ID
   # first header line (remove linebreaks) leer,"Produkt",list product names, leer
   echo ",\"Produkt\",$(sed ':a;N;$!ba;s/\n//g' ${CSV_PROD_NAME})" > $CSVF
   # second header line (remove linebreaks) "UID","Name/Preis",list product price
   echo "\"UID\",\"Name/Preis EUR\",$(sed ':a;N;$!ba;s/\n//g' ${CSV_PROD_PRICE})\"Summe EUR\"" >> $CSVF

# create table with batch file and fetch it
   echo " executing batch file..."
   sqlite3 -csv -separator ',' -init $SQL_Q "${DB}" "SELECT UID, name, $(sed 's/\([0-9][0-9]*,[0-9]*.[0-9]*\)/\"\1EUR\",/g' $CSV_PROD_ID | sed ':a;N;$!ba;s/\n//g') sum FROM \"bill_comp\" ORDER BY name;">> $CSVF
   #sqlite3 -csv -init $SQL_Q "${DB}" "SELECT * FROM \"bill_comp\" ORDER BY name;">> $CSVF

   if [ $? -eq 0 ]; then
      echo "   ${CSVF} written."
   else
      echo "   ERROR: sqlite3 aborted with ${?}: export failed!" 1>&2
      cp $SQL_Q $BKS_LOG_DIR 1>&2
      echo "   check ${SQL_Q}, " 1>&2
   fi


# add sum up row
   echo " creating sum row..."
   # first entries are empty
   echo ",\"Verbrauch Anzahl\"" > $SUM_ROW

   sqlite3 -csv "${DB}" "SELECT count FROM previous_consumption;" >> $SUM_ROW
   sqlite3 -csv "${DB}" "SELECT round(sum(\"sum\"),2) AS summe FROM previous_account_balance;" >> $SUM_ROW
   #cat $SUM_ROW
   # replace newline with ,
   sed ':a;N;$!ba;s/\n/,/g' $SUM_ROW >> $CSVF
   #cat $CSVF

# cleanup
   echo " removing tmp files..."
   rm -r $BKS_TMP_SUB_DIR
   #rm -f $CSV_PROD $PROD_LIST $CSV_USER $CSV_BILL $SUM_ROW #$SQL_Q
