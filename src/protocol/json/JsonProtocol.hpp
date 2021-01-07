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

#ifndef _EMBDB_JSON_PROTOCOL_HPP_
#define _EMDBD_JSON_PROTOCOL_HPP_

#include <nlohmann/json.hpp>

#include "../../database/DbElementType.hpp"
#include "../IProtocol.hpp"

namespace embDB_protocol {

using json = nlohmann::json;

class JsonProtocol : public IProtocol {
 public:
  JsonProtocol();
  virtual ~JsonProtocol();

  // IProtocol
  virtual bool validateInput(embDB_server::ClientBuffer& buffer) override;
  virtual void addInput(embDB_server::ClientBuffer& buffer) override;

  virtual ProtErrorCode translateToDataObject(
      std::unique_ptr<DataObject>& dataobject) override;
  virtual void translateToString(const std::unique_ptr<DataObject>& dataobject,
                                 std::string& data) override;
  virtual void buildError(embDB_errorcode::ErrorCode error,
                          std::string& data) override;

 private:
  std::string m_data;

  const std::string c_command = "command";
  const std::string c_command_clearall = "clearAll";
  const std::string c_command_readrow = "readRow";
  const std::string c_command_readrowtimestamped = "readRowTimestamped";
  const std::string c_command_createrow = "createRow";
  const std::string c_command_deleterow = "deleteRow";
  const std::string c_command_rowexists = "rowExists";
  const std::string c_command_rowcount = "rowCount";
  const std::string c_command_writeitem = "writeItem";

  const std::string c_name = "name";

  const std::string c_type = "type";
  const std::string c_type_string = "string";
  const std::string c_type_uint32 = "uint32";
  const std::string c_type_int32 = "int32";
  const std::string c_type_uint64 = "uint64";
  const std::string c_type_int64 = "int64";
  const std::string c_type_float = "float";
  const std::string c_type_double = "double";
  const std::string c_type_bool = "bool";
  const std::string c_type_bytes = "bytes";

  const std::string c_starttime = "startTime";
  const std::string c_endtime = "endTime";

  const std::string c_itemscount = "itemsCount";

  const std::string c_data = "data";

  const std::string c_error = "error";

  const std::string c_rowcount = "rowCount";
  const std::string c_rowexists = "rowExists";

  const std::string c_dataarray = "values";
  const std::string c_value = "value";
  const std::string c_timestamp = "timestamp";

  bool checkFieldExists(json::object_t& field, std::string key);
  int parseStringToCommand(const std::string& strcommand,
                           ProtocolCommand& protCommand);
  int parseCommandToString(ProtocolCommand protCommand,
                           std::string& strcommand);
  int parseStringToDbElType(const std::string& strType,
                            embDB_database::DbElementType& dataType);
  int parseDbElTypeToString(embDB_database::DbElementType dataType,
                            std::string& strType);
};

}  // namespace embDB_protocol
#endif
