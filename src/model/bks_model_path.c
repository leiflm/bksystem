//      bks_model_path.c
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
#include <stdio.h>
#include <Ecore.h>
#include <Eina.h>
#include "Bks_Model.h"
#include "Bks_System.h"



static Ecore_Thread *_bks_model_path_save(char *path);
static char* _bks_model_path_load();
static void _bks_model_path_save_cb(void *data, Ecore_Thread *th UNUSED);

static const char *fname = BKS_DB_PATH_FILE;

// Path to database
char *_bks_model_path_get(void) {

   char *path;
   char *default_path = BKSYSTEMDB;
   size_t len;
   path = _bks_model_path_load();
   if (!path) {
      printf("loading default path\n");
      // no path found: set default path and name
      len = strlen(BKSYSTEMDB) + 6;
      path = malloc(len);
      snprintf(path, len, "file:%s",  default_path);
   }
   return path;
   }

void _bks_model_path_set(Eina_Stringshare *path) {
   char *chpath, *newpath;
   
   if (!mdl.db_path) {
      eina_stringshare_del(mdl.db_path);
   }
   chpath = (char*)path;
   newpath = calloc(1, 256);
   strncpy(newpath, chpath, 256);
   mdl.db_path = newpath;
   
   _bks_model_path_save((char*)path);
}

static char *_bks_model_path_load() {

   char *path = NULL;   
   FILE *fp;
   int c,i;
   path = calloc(256,1);
   fp = fopen(fname, "r");
   i = 0;
   if (fp) {
      c = fgetc(fp);
      while (c != EOF && i < 255) {
         *(path + i) = c;
         i++;
         c = fgetc(fp);   
      }
      *(path + i) = '\0';
      fclose(fp);
   } else {
      fprintf(stderr, "cannot open file: %s \n", fname);
      free(path);
      path = NULL;
   }
   return path;
}

// Save path async
static void _bks_model_path_save_cb(void *data, Ecore_Thread *th UNUSED) {

   char *path = (char*)data;
   FILE *fp;


   if ((path)) {
      fprintf(stderr, "Path: %s saved to file %s \n", path, fname);

      fp = fopen(fname, "w");
      if (fp) {
         while (*(path) != '\0') {
            fputc(*(path++), fp);
         }
      fclose(fp);
      } else {
         fprintf(stderr, "cannot open path file: %s \n", fname);
      }
   } else {
      fprintf(stderr, "Returned Path is (null)\n");
   }
}

static Ecore_Thread *_bks_model_path_save(char *path) {

   return ecore_thread_run(_bks_model_path_save_cb, NULL, NULL, (void*)path);
}
