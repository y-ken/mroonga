/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2010 Tetsuro IKEDA
  Copyright(C) 2011-2012 Kentoku SHIBA
  Copyright(C) 2011-2012 Kouhei Sutou <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "mrn_path_mapper.hpp"

#include <string.h>

namespace mrn {
  PathMapper::PathMapper(const char* mysql_path)
    : mysql_path_(mysql_path) {
    db_path_[0] = '\0';
    db_name_[0] = '\0';
  }

  /**
   * "./${db}/${table}"                              ==> "${db}.mrn"
   * "./${db}/"                                      ==> "${db}.mrn"
   * "/tmp/mysql-test/var/tmp/mysqld.1/#sql27c5_1_0" ==>
   *   "/tmp/mysql-test/var/tmp/mysqld.1/#sql27c5_1_0.mrn"
   */
  const char* PathMapper::db_path() {
    if (db_path_[0] != '\0') {
      return db_path_;
    }

    if (strncmp(mysql_path_, "./", 2) == 0) {
      int i = 2, j = 0, len;
      len = strlen(mysql_path_);
      while (mysql_path_[i] != '/' && i < len) {
        db_path_[j++] = mysql_path_[i++];
      }
      db_path_[j] = '\0';
    } else {
      strcpy(db_path_, mysql_path_);
    }
    strcat(db_path_, MRN_DB_FILE_SUFFIX);
    return db_path_;
  }

  /**
   * "./${db}/${table}"                              ==> "${db}"
   * "./${db}/"                                      ==> "${db}"
   * "/tmp/mysql-test/var/tmp/mysqld.1/#sql27c5_1_0" ==>
   *   "/tmp/mysql-test/var/tmp/mysqld.1/#sql27c5_1_0"
   */
  const char* PathMapper::db_name() {
    if (db_name_[0] != '\0') {
      return db_name_;
    }

    if (strncmp(mysql_path_, "./", 2) == 0) {
      int i = 2, j = 0, len;
      len = strlen(mysql_path_);
      while (mysql_path_[i] != '/' && i < len) {
        db_name_[j++] = mysql_path_[i++];
      }
      db_name_[j] = '\0';
    } else {
      strcpy(db_name_, mysql_path_);
    }
    return db_name_;
  }
}
