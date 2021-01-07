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

#ifndef _EMBDB_ERROR_CODES_HPP_
#define _EMBDB_ERROR_CODES_HPP_

#include "../errorcode/ErrorCode.hpp"

namespace embDB_protocol {

enum ProtErrorCode {
  SUCCESS = 0,
  NO_COMMAND = embDB_errorcode::ErrorCodeDefiner::START_PROT_ERROR,
  UNKNOWN_COMMAND,
  NO_NAME,
  NO_TYPE,
  UNKNOWN_TYPE,
  NO_STARTTIME,
  NO_ENDTIME,
  NO_ITEMSCOUNT,
  NO_DATA,
  INVALID_DATA,
};

}

#endif