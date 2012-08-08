#!/bin/sh
# This script is intended to export a bill from bksystem.sqlite database to html.
# It will generate a user(rows) X products(colums) pivot table, with count as data.
# It will tag some fields to get some formating with css style-sheets
#
# Steps:
#
# get current date and time
# export tables user, product and bill as .csv
# generate sqlfile with statements for:
#   CREATE TABLE with products as columns
#   INSERT user rows (each user)
#   UPDATE user rows with consumption (each user and product)
# query table and add 2 first and last lines as html
# export table as .html
# add tag to certain fields (e.g. sum, uid, etc.
# cleanup af tmp files 

   echo ""
   echo "Executing script file:"
   echo "${0}:"
   echo " Started exporting complete bill..."

# CONFIG: here you can set the entries for certain field e.g. total sum etc.

   # 1./2./last row  #  field 1/2/last
   R1F1=""; R1F2="Produkte"; R1FL=""
   R2F1="UID"; R2F2="Name/Preis in &euro;"; R2FL="Summe &euro;"
   LRF1=""; RLF2="Verbrauch Anzahl"

   # layout to use (will be embedded)
   LAYOUT="${BKS_SCR_DIR}/layout.css" 

   # subscript all html after </table> except both </body> and </html>, if too long replace with file
   SUB_TEXT="<p>Karlsruhe, den $(date +%0d.%0m.%0Y), Bierwart</p>"
   SUBTEXTF="${BKS_SCR_DIR}/sub_txt_tmpl" # also replace occurence of $SUB_TEXT with " sed 's/DATE_PLACEHOLDER/"$DATEPRINT"/g' <$SUBTEXTF >>$HTMLF"

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
   DATEPRINT="${DATE:8:2}.${DATE:5:2}.${DATE:0:4} ${DATE:11:8}"

   # table caption
   TBL_CAP="Abrechnung vom ${DATEPRINT}"

#filenames + paths for the exported files, should be defined, if not:
   # exported will be written here
   if [ -z $BKS_BILL_DIR ]; then
      BKS_BILL_DIR="."
   fi
   FILENAME="Abrechnung"
   HTMLF="${BKS_BILL_DIR}/${FILENAME}_${DATEFILE}_table.html"
   #HTMLF="${BKS_BILL_DIR}/gen_table.html"
   echo " Filename:"
   echo "   ${HTMLF}"


   # set tmp directory
   if [ -z $BKS_TMP_DIR ]; then
      BKS_TMP_DIR="."
   fi
   BKS_TMP_SUB_DIR="${BKS_TMP_DIR}/html"
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
      SQL_Q="${BKS_TMP_SUB_DIR}/create_querry_html.sql"
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

#HTML GENERATION

   # input templates
   echo "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" >$HTMLF
   echo "<html><head>" >>$HTMLF
   echo "<META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">" >>$HTMLF
   echo "<title>${TBL_CAP}</title>" >>$HTMLF
   cat $LAYOUT >>$HTMLF
   #echo "<link rel=\"stylesheet\" type=\"text/css\" media=\"screen\" href=\"../styles-screen.css\">">>$HTMLF
   #echo "<link rel=\"stylesheet\" type=\"text/css\" media=\"print\" href=\"../styles-print.css\">">>$HTMLF
   echo "</head><body>" >>$HTMLF
   # may replaced by template file
   
   # insert head line
   echo "<h3>${TBL_CAP}</h3>" >>$HTMLF
   echo "<table id=\"bill\">" >>$HTMLF
   
# create 2 header lines/ 1.replace EANs with name 2. with price
   echo " creating table header..."
   # get all sold product with name and price, order by ID
   sqlite3 -csv -separator '#' $DB "SELECT products.name, previous_consumption.price FROM previous_consumption, products WHERE products.EAN=previous_consumption.EAN ORDER by products.EAN;" > $CSV_PROD_NAME_ID
   # parse list, remove quotes around name, new quotes around combined name+price, append to new list

   while IFS='#' read -r CUR_PROD_NAME CUR_PROD_PRICE ; do
      # this line does the rotation also esp. width=\"16\" height=\"160\" AND rotate(270, 12, 0) translate(-140,0) height+ translat are in relation
      echo "<TD class=\"prod_name\"><svg width=\"16\" height=\"116\"><text id=\"prod_name_rot\" transform=\"rotate(270, 12, 0) translate(-100,0)\">$(echo ${CUR_PROD_NAME} | sed 's/\"//g' )</text></svg></TD>"  >> $CSV_PROD_NAME
      echo "<TD class=\"prod_price\">${CUR_PROD_PRICE}</TD>"  >> $CSV_PROD_PRICE
   done < $CSV_PROD_NAME_ID
   #cat $CSV_PROD_NAME 
   #cat $CSV_PROD_PRICE
   #echo "<COLGROUP #class=\"sum_col\"></COLGROUP>" >>$HTMLF 

   echo "<thead>" >>$HTMLF
   # first header line (remove linebreaks) leer,"Produkt",list product names, leer
   echo "<TR id=\"prod_names_row\">" >>$HTMLF
   echo "<TD>${R1F1}</TD>" >>$HTMLF
   echo "<TD>${R1F2}</TD>" >>$HTMLF
   echo "$(cat ${CSV_PROD_NAME})" >>$HTMLF
   echo "<TD>${R1FL}</TD></TR>" >>$HTMLF
   
   # second header line (remove linebreaks) "UID","Name/Preis",list product price
   echo "<TR id=\"prod_price_row\">" >>$HTMLF
   echo "<TD>${R2F1}</TD>" >>$HTMLF
   echo "<TD>${R2F2}</TD>" >>$HTMLF
   echo "$(cat ${CSV_PROD_PRICE}) " >>$HTMLF
   echo "<TD class=\"sum\", id=\"sum_text\">${R2FL}</TD></TR>" >>$HTMLF


   echo "</thead>" >>$HTMLF
   echo "</tbody>" >>$HTMLF
   #cat $CSVF

   
# create table with batch file and fetch it
   echo " executing batch file..."
   # get as html: recreate column IDs from csv: "EAN,priceEUR", then remove linebreaks, => insert it to the SELECT statement
   # then search for (.Number) append zero (e.g. 0.9 => 0.90, 0.95 => 0.95)  
   sqlite3 -html  -init $SQL_Q "${DB}" "SELECT UID, name, $(sed 's/\([0-9][0-9]*,[0-9]*.[0-9]*\)/\"\1EUR\",/g' $CSV_PROD_ID | sed ':a;N;$!ba;s/\n//g') sum FROM \"bill_comp\" ORDER BY name;" | sed 's/\([.][0-9]\)\([^0-9]\)/\10\2/g' >>$HTMLF                   
   #sqlite3 -csv -init $SQL_Q "${DB}" "SELECT * FROM \"bill_comp\" ORDER BY name;">>$HTMLF

   # TODO check will fail cause $? is exitcode of sed, not sqlite3
   if [ $? -eq 0 ]; then
      echo "   ${HTMLF} written."
   else
      echo "   ERROR: sqlite3 aborted with ${?}: export failed!" 1>&2
      cp $SQL_Q $BKS_LOG_DIR 1>&2
      echo "   check ${SQL_Q}" 1>&2
   fi

   echo "</tbody>" >>$HTMLF
   echo "<tfoot>" >>$HTMLF
   
# add sum up row
   echo " creating sum row..."
   # first entries are empty
   echo "<TR id=\"sum_row\">" >>$HTMLF
   echo "<TD>${LRF1}</TD>" >>$HTMLF
   echo "<TD>${RLF2}</TD>" >>$HTMLF

   sqlite3 -html "${DB}" "SELECT count FROM previous_consumption;" >$SUM_ROW

   #cat $SUM_ROW
   # kick TR tags, ,
   sed 's/<TR>//g' $SUM_ROW | sed 's/<\/TR>//g' | sed 's/<TD>/<TD class=\"sum_row_fields\">/g' >>$HTMLF
   sqlite3 -html "${DB}" "SELECT round(sum(\"sum\"),2) AS summe FROM previous_account_balance;"  \
   | sed 's/<TR>//g' | sed 's/<\/TR>//g' | sed 's/<TD>/<TD id=\"sum_col_row\">/g' >>$HTMLF


   echo "</TR>" >>$HTMLF
   echo "</tfoot>" >>$HTMLF
   echo "</table>" >>$HTMLF
   # text $SUB_TEXT
   #echo "${SUB_TEXT}" >>$HTMLF
   # text in file $SUB_TEXT
   # insert current! not BKS_DATE
   sed s/DATE_PLACEHOLDER/"$(date +%d.%m.%Y)"/g < $SUBTEXTF >> $HTMLF
   echo "</body></html>" >>$HTMLF


   #   cat $CSVF
# cleanup
   echo " removing tmp files..."
   rm -r $BKS_TMP_SUB_DIR
   #rm $CSV_PROD $PROD_LIST $CSV_PROD_ID $PROD_LIST $CSV_USER $CSV_BILL $SQL_Q $CSV_PROD_NAME $CSV_PROD_PRICE $CSV_PROD_NAME_ID $SUM_ROW
