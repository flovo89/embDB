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

#ifndef _EMBDB_I_PROTOCOL_HPP_
#define _EMBDB_I_PROTOCOL_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "../errorcode/ErrorCode.hpp"
#include "../server/buffer/ClientBuffer.hpp"
#include "DataObject.hpp"
#include "ProtErrorCode.hpp"

namespace embDB_protocol {

class IProtocol {
 public:
  IProtocol();
  virtual ~IProtocol();

  virtual bool validateInput(embDB_server::ClientBuffer& buffer) = 0;
  virtual void addInput(embDB_server::ClientBuffer& buffer) = 0;

  virtual ProtErrorCode translateToDataObject(
      std::unique_ptr<DataObject>& dataobject) = 0;
  virtual void translateToString(const std::unique_ptr<DataObject>& dataobject,
                                 std::string& data) = 0;
  virtual void buildError(embDB_errorcode::ErrorCode error,
                          std::string& data) = 0;
};

}  // namespace embDB_protocol
#endif
