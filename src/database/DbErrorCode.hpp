/*
 * embDB
 * Copyright (C) 2021 flovo89
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _EMBDB_DB_ERROR_CODE_HPP_
#define _EMBDB_DB_ERROR_CODE_HPP_

namespace embDB_database {

enum DbErrorCode {
  SUCCESS = 0,
  NOTFOUND = -1,
  INTERNAL = -2,
  HASHNAMEMISMATCH = -3,
  ITEMSCOUNTMISMATCH = -4,
  TYPEMISMATCH = -5,
};

}

#endif