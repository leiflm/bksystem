#!/bin/sh
# bksystem admin script.
# Structure of this file:
#     Help message
#     Config: paths, filenames, enabled scripts
#
#     setup logfile
#     get command line options
#     set/test DB
#     set date
#     set other paths/filenames
#     enable/disable scripts
#     exec scripts
#     cleanup

   VERSION=0.9

   help_message()
   {
      echo "Usage:"
      echo "   ${0} [options] [-f DBNAME]"
      echo "   ${0} [options]"
      echo "Functions: "
      echo "   set environment for all bksystem scripts: generate bill, export, backup, cleanup "
      echo ""
      echo "Options:"
      echo ""
      echo "  -f : use DBNAME as database"
      echo "          e.g. -f myDB.sqlite"
      echo "  -l : send output to stdout (console) and NOT logfile"
      echo "       all info commands imply this behavior"
      echo ""
      echo "  -a : exec all configured functions, overwrites other options" 
      echo "  -b : enable backup, not with -r"
      echo "  -e : enable export, not with -r"
      echo "  -i : enable insert reference date for bill, not with -r"
      echo "  -r : restore database from backup (old will not be overwritten), performs no other action"
      echo ""
      echo "  -s : show set variables and exit"
      echo "       tmp variables will be set to default, unless you set them first:"
      echo "         e.g. export BKS_BILL_DIR=/home/user/Path/to/bills"
      echo "  -c : show config and exit"
      echo "       config can be set in $pwd/${0}"
      echo "  -d : show default variables and exit"
      echo "       default variables: can be set in ${0}"
      echo "       usabale for defining tmp variables"
      echo "          e.g. export BKS_TMP_DIR=\${DEF_ROOT_DIR}/mytmp" 
      echo ""
      echo "  -h : show this help and exit"
      echo "       uses same path and name as reading a DB"
      echo "  -v : display version and exit"
      exit 0;
   }

# Configuration

   # Default variables, should be set while install

      # adjust if necessary (for crontab) or permanent links
      # for temorary links overwrite with export (all BKS_* will be set to $DEF_* unless already set) 
      DEF_ROOT_DIR="${HOME}/Projekte/bksystem/build"
      # Database
      DEF_DB_DIR="${DEF_ROOT_DIR}/build"
      DEF_DB_NAME="bksystem.sqlite"
      # Scripts are located here
      DEF_SCR_DIR="${DEF_ROOT_DIR}/scripts"
      # bills
      DEF_ADD_DATE_BILL_DIR=1 # USE subdir and add the current date to the BILL_SUB_DIR_NAME: 0 disabled/ 1 enabled
                              # get something like DEF_ROOT_DIR/DEF_BILL_DIR_NAME/DEF_SUB_DIR_NAME_DATE/file.csv
                              #
      DEF_BILL_SUB_DIR_NAME="Abrechnungen" #create a subdir for each bill: disable subdir with "."
      DEF_BILL_DIR="${DEF_ROOT_DIR}/build/bills"
      # temporary dir
      DEF_TMP_DIR="${DEF_ROOT_DIR}/build/tmp"
      # here will be the log file
      DEF_LOG_DIR="${DEF_ROOT_DIR}/build/log" # send to console with -c option
      DEF_LOG_FILE="bill_script.log"
      # here will be the backup
      DEF_BKP_DIR="${DEF_ROOT_DIR}/build/bkp"

   # enable/disable scripts permanently here, tmp use options
      # enabled: VARNAME=1
      # disabled VARNAME=0

      FIXBILLDATE=1
      EXPSIMPLE=1
      EXPCOMPLCOMB=1
      EXPCOMPLSEP=0
      EXPCOMPLHTML=1
      BACKUP=0
      CLEAN=0 # TODO
      #always disabled, enable with -r switch
      RESTORE=0

   # more messages
      show_default_variables()
      {
         echo "default variables: can be set in ${0}"
         echo "usabale for defining tmp variables"
         echo "e.g. export BKS_TMP_DIR=\${DEF_ROOT_DIR}/mytmp" 
         echo "   DEF_ROOT_DIR=${DEF_ROOT_DIR}"
         echo "   DEF_DATE=${DEF_DATE} (now or last inserted date!)"
         echo "   DEF_DB_DIR=${DEF_DB_DIR}"
         echo "   DEF_DB_NAME=${DEF_DB_NAME}"
         echo "   DEF_BILL_DIR=${DEF_BILL_DIR}"
         echo "   DEF_TMP_DIR=${DEF_TMP_DIR}"
         echo "   DEF_LOG_DIR=${DEF_LOG_DIR}"
         echo "   DEF_LOG_FILE=${DEF_LOG_FILE}"
         echo "   DEF_BKP_DIR=${DEF_BKP_DIR}"
         echo "   DEF_SCR_DIR=${DEF_SCR_DIR}" 
      }

      show_set_variables()
      {
         echo "These tmp variables will be set to default, unless you set them:"
         echo "   BKS_DATE=${BKS_DATE}"
         echo "   BKS_TMP_DIR=${BKS_TMP_DIR}"
         echo "   BKS_BILL_DIR=${BKS_BILL_DIR}"
         echo "   BKS_LOG_DIR=${BKS_LOG_DIR}"
         echo "   BKS_LOG_FILE=${BKS_LOG_FILE}"
         echo "   BKS_BKP_DIR=${BKS_BKP_DIR}"
         echo "   BKS_DB_DIR=${BKS_DB_DIR}"
         echo "   BKS_DB_NAME=${BKS_DB_NAME}"
         echo "   BKS_SCR_DIR=${BKS_SCR_DIR}"
      }

      show_conf()
      {
         echo "Following tasks are enabled=0, disabled=1"
         echo " to set, edit ${0}"  
         echo " insert_bill_reference_date.sh  : ${FIXBILLDATE}"
         echo " export_bill_simple.sh: ${EXPSIMPLE}"
         echo " export_bill_list_comb: ${EXPCOMPLCOMB}"
         echo " export_bill_list_sep : ${EXPCOMPLSEP}"
         echo " export_bill_list_html: ${EXPCOMPLHTML}"
         echo " backup.sh            : ${BACKUP}"
         echo " cleanup.sh           : ${CLEAN}"
         echo " restore_db.sh        : ${RESTORE}"
      }

   # dont edit this, default date is current date
      DEF_DATE=$(date +"%0Y-%0m-%0d_%0k:%0M:%0S")

   # check for help
      case $1 in
         -h | -help | --h | --help | -usage | --usage | usage | help | h) help_message ;;
      esac

# setup logfile/output

      if [ -z $BKS_LOG_DIR ]; then
         BKS_LOG_DIR=$DEF_LOG_DIR
      fi
      if [ -z $BKS_LOG_FILE ]; then
         BKS_LOG_FILE=$DEF_LOG_FILE
      fi 
      #if [ -z $BKS_LOG ]; then
         #BKS_LOG="${DEF_LOG_DIR}/${BKS_DATE}_${BKS_LOG_FILE}"
         BKS_LOG="${BKS_LOG_DIR}/${BKS_LOG_FILE}"
         BKS_TMP_LOG="${BKS_LOG_DIR}/${BKS_LOG_FILE}_tmp"
      #fi
      if ! [ -d $BKS_LOG_DIR ]; then
         mkdir $BKS_LOG_DIR
      fi

      echo "LOG on scripted bill generation for bksystem:" > $BKS_TMP_LOG 2>&1 
      echo "${0} ${@}, version: ${VERSION}" >> $BKS_TMP_LOG 2>&1 
      echo "${DEF_DATE}" >> $BKS_TMP_LOG 2>&1 

      # test sqlite3 command exists
      command -v $PROGS > /dev/null || { echo "ERROR: tee not found. Please install it. Aborting!" >> $BKS_TMP_LOG 2>&1; echo "ERROR: tee not found. Please install it. Aborting!"; mv $BKS_TMP_LOG $BKS_LOG 2>/dev/null; exit 1; }
      for PROGS in sqlite3 getopts; do
         command -v $PROGS > /dev/null || { echo "ERROR: ${PROGS} not found. Please install it. Aborting!" 2>&1 | tee -a $BKS_TMP_LOG; mv $BKS_TMP_LOG $BKS_LOG 2>/dev/null; exit 1; } 
      done

   # clear these, used to remember chosen options
      ENEXP=0
      ENBKP=0
      ENINS=0
      ENINSERT=0
      EXECALL=0
      DB=
      OPTION_SELECTED=0
      NOLOG=0

# get commandline args
      while getopts abcdef:hilrsv opt
         do
         case $opt in
            a) OPTION_SELECTED=1; EXECALL=1 ;;
            b) OPTION_SELECTED=1; ENBKP=1 ;;
            c) OPTION_SELECTED=1; show_conf; NOLOG=1 ; exit 0;;
            d) OPTION_SELECTED=1; show_default_variables; BKS_LOG=/dev/null ; exit 0;;
            e) OPTION_SELECTED=1; ENEXP=1 ;;
            f) OPTION_SELECTED=1; DB=$OPTARG; DBARG="1" ;;
            h) OPTION_SELECTED=1; help_message;;
            i) OPTION_SELECTED=1; ENINSERT=1 ;;
            l) OPTION_SELECTED=1; NOLOG=1  ;;
            s) OPTION_SELECTED=1; show_set_variables;NOLOG=1  ; exit 0;;
            r) OPTION_SELECTED=1; RESTORE=1 ;;
            v) OPTION_SELECTED=1; echo "version: ${VERSION}"; NOLOG=1  ; exit 0;;
            *) echo "unrecognized option";BKS_LOG=/dev/null ; help_message; exit 0;;
         esac
      done
      # if logfile disabled
      if ! [ $NOLOG -eq 1 ]; then
         mv $BKS_TMP_LOG $BKS_LOG
      else
         rm $BKS_TMP_LOG
         $BKS_LOG=/dev/null
      fi
   # set all path and name variables that are not yet defined to default
   # some need the date current date OR the last inserted date, therefore init DB and log first, then get date, init rest. 


# set DB to use
      # set DB vars to default if not set
      if [ -z $BKS_DB_DIR ]; then
         BKS_DB_DIR=$DEF_DB_DIR
      fi
      if [ -z $BKS_DB_NAME ]; then
         BKS_DB_NAME=$DEF_DB_NAME
      fi
      # set DB from commandline
      if [ -z $DB ]; then
         DB="${BKS_DB_DIR}/${BKS_DB_NAME}"
      else
         echo "option: DB from -f argument"  2>&1 | tee -a $BKS_LOG
      fi
      # test DB exist
      if [ -e $DB ]; then
         echo "Database:"  2>&1 | tee -a $BKS_LOG
         echo $DB  2>&1 | tee -a $BKS_LOG
      else
         if [ 0 -eq $RESTORE ]; then 
            echo "ERROR: Database ${DB} not found. Please adjust settings in ${SCR_DIR}/${0}. Aborting!"  2>&1 | tee -a $BKS_LOG
            exit 1
         else
            echo "DB not existent: restoring..." 2>&1 | tee -a $BKS_LOG
         fi
      fi
      export DB
   
# set date
      # now get current date to insert or read from DB
      if ! [ $RESTORE -eq 1 ]; then 
         if ([ $ENINSERT -eq 1 ] || [ $FIXBILLDATE -eq 1 ]); then
            # declared DATE via EXPORT
            if [ -z $BKS_DATE ]; then
               BKS_DATE=$DEF_DATE
            fi
         else
            # get last date, if we dont insert current date (make a bill now)
            # replace space with _ and remove "
            BKS_DATE=$(sqlite3 -csv database/bksystem.sqlite "SELECT max(timestamp) FROM bill_dates;" | sed 's/\ /_/g' | sed 's/\"//g')
         fi
      else
         BKS_DATE=$DEF_DATE
      fi
      export BKS_DATE

#set directories and paths
      # Scripts are located here
      if [ -z $BKS_SCR_DIR ]; then
         export BKS_SCR_DIR=$DEF_SCR_DIR
      fi

      # Here go the generated/exported files
      # complete bill path
      if [ -z $BKS_BILL_DIR ]; then
         BKS_BILL_DIR="${DEF_BILL_DIR}"
      fi
      if ! [ -d $BKS_BILL_DIR ]; then
         mkdir $BKS_BILL_DIR
      fi
      # complete bill path + subdir
      if [ 1 -eq $DEF_ADD_DATE_BILL_DIR ]; then
         BKS_BILL_DIR="${BKS_BILL_DIR}/${DEF_BILL_SUB_DIR_NAME}_${BKS_DATE}"
         if ! [ -d $BKS_BILL_DIR ]; then
            mkdir $BKS_BILL_DIR
         fi
      fi
      export BKS_BILL_DIR

   # temporary dir
      if [ -z $BKS_TMP_DIR ]; then
         export BKS_TMP_DIR=$DEF_TMP_DIR
      fi
      if ! [ -d $BKS_TMP_DIR ]; then
         mkdir $BKS_TMP_DIR
      fi
      
   # backup: here will be the backup
      if [ -z $BKS_BKP_DIR ]; then
         export BKS_BKP_DIR=$DEF_BKP_DIR
      fi
      if ! [ -d $BKS_BKP_DIR ]; then
         mkdir $BKS_BKP_DIR
      fi

# enable/disable scripts
      if [ 0 -eq $OPTION_SELECTED ]; then
         echo "no OPTION selected: use ${0} -a to perform all following tasks:"
         show_conf;
         echo "or  use ${0} -h to get help"
         exit 1
      fi

   #log selected options
   # and disable unselected actions
      # restore will overwrite all -a , -e, -i, -b (all, export, insert, backup)
      if [ 1 -eq $RESTORE ]; then
         echo "OPTION: restore DB, will not perform other tasks!" 2>&1 | tee -a $BKS_LOG
      else
      # -a
         if [ 1 -eq $EXECALL ]; then
            echo "OPTION: performing all configured tasks"
            RESTORE=0
         else
            FIXBILLDATE=0
            EXPSIMPLE=0
            EXPCOMPLCOMB=0
            EXPCOMPLSEP=0
            EXPCOMPLHTML=0
            BACKUP=0
            CLEAN=0
         fi
         # process enable options
         if [ 1 -eq $ENINSERT ]; then
            echo "OPTIONS: enable insert reference date"  2>&1 | tee -a $BKS_LOG
            FIXBILLDATE=1
         fi
         if [ 1 -eq $ENEXP ]; then
            echo "OPTION: enable export"  2>&1 | tee -a $BKS_LOG
            EXPSIMPLE=1
            EXPCOMPLCOMB=1
            EXPCOMPLSEP=1
            EXPCOMPLHTML=1
         fi
         if [ 1 -eq $ENBKP ]; then
            echo "OPTION: enable backup"  2>&1 | tee -a $BKS_LOG
            BACKUP=1
         fi
         show_set_variables 2>&1 | tee -a $BKS_LOG
      fi
      echo "Starting scripts from ${0}"  2>&1 | tee -a $BKS_LOG


# Here are all the scripts
   # to enable/disable scripts set variables at start of script
      # saves return code of scripts, cause pipe tee destroys it. 
      export ERRCODE="${BKS_TMP_DIR}/errcode"

      if ! [ 1 -eq $RESTORE ]; then
         if [ 1 -eq $FIXBILLDATE ]; then
            $BKS_SCR_DIR/insert_bill_reference_date_into_db.sh 2>&1 | tee -a $BKS_LOG
            echo $REPLY
            if ! [ $(cat $ERRCODE) = "0" ]; then
               echo "ERROR: inserting date unsuccesful: insert_date_to_db.sh failed, ABORTING" | tee -a $BKS_LOG
               echo "${DATE}" > "${BKS_BILL_DIR}/ERROR_README.txt"
               echo "Invoked command ${0} ${@}, version ${VERSION}" >> "${BKS_BILL_DIR}/ERROR_README.txt"
               echo "Error while inserting date, see ${BKS_LOG} for more Info" >> "${BKS_BILL_DIR}/ERROR_README.txt"
               echo "Try command ${0} -i" >> "${BKS_BILL_DIR}/ERROR_README.txt"
               exit 1;
            fi
         fi
         # exports current_bill/consumption
         if [ 1 -eq $EXPSIMPLE ]; then
            $BKS_SCR_DIR/export_bill_basic.sh  2>&1 | tee -a $BKS_LOG
         fi
         # generate komplex bill X consumption pivot table and exports csv
         if [ 1 -eq $EXPCOMPLCOMB ]; then
            $BKS_SCR_DIR/export_bill_complete_comb.sh  2>&1 | tee -a $BKS_LOG
         fi
         # generate komplex bill X consumption pivot table and exports csv (seperate last/firstname)
         if [ 1 -eq $EXPCOMPLSEP ]; then
            $BKS_SCR_DIR/export_bill_complete_sep.sh  2>&1 | tee -a $BKS_LOG
         fi
         # generate komplex bill X consumption pivot table and exports csv
         if [ 1 -eq $EXPCOMPLHTML ]; then
            $BKS_SCR_DIR/export_bill_html.sh 2>&1 | tee -a $BKS_LOG
         fi
         # backups useraccounts and products
         if [ 1 -eq $BACKUP ]; then
            $BKS_SCR_DIR/backup.sh  2>&1 | tee -a $BKS_LOG
         fi
         # deletes unecessary sales
         if [ 1 -eq $CLEAN ]; then
            $BKS_SCR_DIR/delete_old_sales.sh  2>&1 | tee -a $BKS_LOG
         fi
      else
         $BKS_SCR_DIR/restore_db.sh  2>&1 | tee -a $BKS_LOG
      fi

#cleanup

   #if [ $1 ]; then
   #remove errcode file
      #rm $ERRCODE >/dev/null 2>&1
      #remove old log (if not overwritten)   

   #remove tmp files (don't remove tmp, could be system tmp)
   #   rm $BKS_TMP_DIR/*
   #fi
   echo "All tasks completed!"  2>&1 | tee -a $BKS_LOG
