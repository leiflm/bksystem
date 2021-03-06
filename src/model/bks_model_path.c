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
#include <Ecore_File.h>
#include <Eina.h>
#include "Bks_Model.h"
#include "Bks_System.h"

#define MAX_PATH 256


static Ecore_Thread *_bks_model_path_save(char *path);
static void _bks_model_path_save_cb(void *data, Ecore_Thread *th UNUSED);

static char *fname = NULL;

// Path to database
void _bks_model_path_set(char *path) {
   char *newpath;
   
   free(mdl.db_path);
   if (path) {
      newpath = calloc(1, MAX_PATH);
      strncpy(newpath, path, MAX_PATH);
      mdl.db_path = newpath;
   
   _bks_model_path_save(newpath);
   } else {
      fprintf(stderr, "Path to set and save is (null)\n");
   }
}

char *_bks_model_path_get(void) {

   char *path = NULL;   
   FILE *fp;
   int c,i;
   path = calloc(MAX_PATH,1);
   fname = calloc(MAX_PATH,1);
   snprintf(fname, (MAX_PATH - 1), "%s/bksystem/" BKS_DB_PATH_FILE, efreet_config_home_get());
   if (!ecore_file_exists(fname))
      ecore_file_mkpath(ecore_file_dir_get(fname));
   fp = fopen(fname, "r");
   i = 0;
   if (fp) {
      c = fgetc(fp);
      while (c != EOF && c != '\n' && i < (MAX_PATH - 1)) {
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

#ifdef DEBUG
   fprintf(stderr, "Path: %s saved to file %s \n", path, fname);
#endif

   fp = fopen(fname, "w");
   if (fp) {
       fputs(path, fp);
       fputc('\n', fp);
       fclose(fp);
   } else {
      fprintf(stderr, "cannot open path file: %s \n", fname);
   }
}

static Ecore_Thread *_bks_model_path_save(char *path) {

   return ecore_thread_run(_bks_model_path_save_cb, NULL, NULL, (void*)path);
}
