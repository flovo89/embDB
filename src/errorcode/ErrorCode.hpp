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

#ifndef _EMBDB_ERROR_CODE_HPP_
#define _EMBDB_ERROR_CODE_HPP_

namespace embDB_errorcode {

enum ErrorCodeDefiner {
  SUCCESS = 0,
  START_DB_ERROR = 10,
  START_PROT_ERROR = 50,
};

typedef int ErrorCode;

static const ErrorCode ERROR_SUCCESS = 0;

}  // namespace embDB_errorcode

#endif