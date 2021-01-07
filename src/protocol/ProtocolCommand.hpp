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

#ifndef _EMBDB_PROTOCOL_COMMANDS_HPP_
#define _EMBDB_PROTOCOL_COMMANDS_HPP_

namespace embDB_protocol {

enum ProtocolCommand {
  NONE = 0,
  READ_ROW = 1,
  READ_ROW_TIMESTAMPED = 2,
  CLEAR_ALL = 3,
  CREATE_ROW = 4,
  DELETE_ROW = 5,
  CHECK_ROW_EXISTS = 6,
  GET_ROW_COUNT = 7,
  WRITE_ITEM = 8,
};

}

#endif