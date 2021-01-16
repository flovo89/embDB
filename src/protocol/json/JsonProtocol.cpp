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

#include "JsonProtocol.hpp"

#include "../../database/DbErrorCode.hpp"
#include "../../exception/Exception.hpp"
#include "../ProtErrorCode.hpp"

namespace embDB_protocol {

//--------------------------------------------------------------------------------------------
JsonProtocol::JsonProtocol() : m_data("") {}

//--------------------------------------------------------------------------------------------
JsonProtocol::~JsonProtocol() {}

//--------------------------------------------------------------------------------------------
bool JsonProtocol::validateInput(embDB_server::ClientBuffer& buffer) {
  std::string peeked;
  bool ret = buffer.peekFromTo('{', '}', peeked);
  if (!ret) return ret;
  return json::accept(peeked);
}

//--------------------------------------------------------------------------------------------
void JsonProtocol::addInput(embDB_server::ClientBuffer& buffer) {
  if (!buffer.popFromTo('{', '}', m_data))
    EMBDB_THROW("Data invalid! Validate before adding");
}

//--------------------------------------------------------------------------------------------
ProtErrorCode JsonProtocol::translateToDataObject(
    std::unique_ptr<DataObject>& dataobject) {
  dataobject.reset(new DataObject());

  json config = json::parse(m_data);
  json::object_t jsonobj = config.get<json::object_t>();

  // Check for command
  if (!checkFieldExists(jsonobj, c_command)) return ProtErrorCode::NO_COMMAND;
  ProtocolCommand com;
  if (parseStringToCommand(jsonobj[c_command], com))
    return ProtErrorCode::UNKNOWN_COMMAND;
  dataobject->setCommand(com);

  // Rowcount does not need further infos...
  if ((com != ProtocolCommand::GET_ROW_COUNT) &&
      (com != ProtocolCommand::CLEAR_ALL)) {
    // Common things
    if (!checkFieldExists(jsonobj, c_name)) return ProtErrorCode::NO_NAME;
    dataobject->setName(jsonobj[c_name]);

    // Evaluate rest
    switch (com) {
      case ProtocolCommand::DELETE_ROW:
      case ProtocolCommand::CHECK_ROW_EXISTS:
        break;
      case ProtocolCommand::READ_ROW: {
        if (!checkFieldExists(jsonobj, c_type)) return ProtErrorCode::NO_TYPE;
        embDB_database::DbElementType type;
        if (parseStringToDbElType(jsonobj[c_type], type))
          return ProtErrorCode::UNKNOWN_TYPE;
        dataobject->setDbElementType(type);
        break;
      }
      case ProtocolCommand::READ_ROW_TIMESTAMPED: {
        if (!checkFieldExists(jsonobj, c_starttime))
          return ProtErrorCode::NO_STARTTIME;
        dataobject->setStartTime(jsonobj[c_starttime]);
        if (!checkFieldExists(jsonobj, c_endtime))
          return ProtErrorCode::NO_ENDTIME;
        dataobject->setEndTime(jsonobj[c_endtime]);
        if (!checkFieldExists(jsonobj, c_type)) return ProtErrorCode::NO_TYPE;
        embDB_database::DbElementType type;
        if (parseStringToDbElType(jsonobj[c_type], type))
          return ProtErrorCode::UNKNOWN_TYPE;
        dataobject->setDbElementType(type);
        break;
      }
      case ProtocolCommand::CREATE_ROW: {
        if (!checkFieldExists(jsonobj, c_type)) return ProtErrorCode::NO_TYPE;
        embDB_database::DbElementType type;
        if (parseStringToDbElType(jsonobj[c_type], type))
          return ProtErrorCode::UNKNOWN_TYPE;
        dataobject->setDbElementType(type);
        if (!checkFieldExists(jsonobj, c_itemscount))
          return ProtErrorCode::NO_ITEMSCOUNT;
        dataobject->setItemsCount(jsonobj[c_itemscount]);
        break;
      }
      case ProtocolCommand::WRITE_ITEM: {
        if (!checkFieldExists(jsonobj, c_type)) return ProtErrorCode::NO_TYPE;
        embDB_database::DbElementType type;
        if (parseStringToDbElType(jsonobj[c_type], type))
          return ProtErrorCode::UNKNOWN_TYPE;
        dataobject->setDbElementType(type);
        if (!checkFieldExists(jsonobj, c_data)) return ProtErrorCode::NO_DATA;

        switch (dataobject->getDbElementType()) {
          case embDB_database::DbElementType::STRING: {
            if (!jsonobj[c_data].is_string())
              return ProtErrorCode::INVALID_DATA;
            std::string str(jsonobj[c_data].get<std::string>());
            dataobject->setDbElement(embDB_database::DbElement(str));
            break;
          }
          case embDB_database::DbElementType::UINT32: {
            if (!jsonobj[c_data].is_number_unsigned())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<uint32_t>()));
            break;
          }
          case embDB_database::DbElementType::INT32: {
            if (!jsonobj[c_data].is_number_integer())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<int32_t>()));
            break;
          }
          case embDB_database::DbElementType::UINT64: {
            if (!jsonobj[c_data].is_number_unsigned())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<uint64_t>()));
            break;
          }
          case embDB_database::DbElementType::INT64: {
            if (!jsonobj[c_data].is_number_integer())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<int64_t>()));
            break;
          }
          case embDB_database::DbElementType::FLOAT: {
            if (!jsonobj[c_data].is_number_float())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<float>()));
            break;
          }
          case embDB_database::DbElementType::DOUBLE: {
            if (!jsonobj[c_data].is_number_float())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<double>()));
            break;
          }
          case embDB_database::DbElementType::BOOL: {
            if (!jsonobj[c_data].is_boolean())
              return ProtErrorCode::INVALID_DATA;
            dataobject->setDbElement(
                embDB_database::DbElement(jsonobj[c_data].get<bool>()));
            break;
          }
          case embDB_database::DbElementType::BYTES: {
            if (!jsonobj[c_data].is_array()) return ProtErrorCode::INVALID_DATA;
            std::vector<uint8_t> vec(
                jsonobj[c_data].get<std::vector<uint8_t>>());
            dataobject->setDbElement(embDB_database::DbElement(vec));
            break;
          }
          default: {
            EMBDB_THROW("Internal error when evaluating dbelementtype");
          }
        }
        break;
      }
      default: {
        EMBDB_THROW("Internal error when parsing command");
      }
    }
  }

  return ProtErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
void JsonProtocol::translateToString(
    const std::unique_ptr<DataObject>& dataobject, std::string& data) {
  json j;
  std::string command;

  if (parseCommandToString(dataobject->getCommand(), command))
    EMBDB_THROW("Internal error when parsing command to string");
  j[c_command] = command;
  j[c_error] = dataobject->getErrorCode();

  if (dataobject->getErrorCode() != embDB_errorcode::ERROR_SUCCESS) {
    data = j.dump();
    return;
  }

  if (dataobject->getCommand() == GET_ROW_COUNT) {
    j[c_rowcount] = dataobject->getRowCount();
  } else if (dataobject->getCommand() != CLEAR_ALL) {
    j[c_name] = dataobject->getName();

    if (dataobject->getCommand() == CHECK_ROW_EXISTS) {
      j[c_rowexists] = dataobject->getRowExists();
    } else if ((dataobject->getCommand() == READ_ROW_TIMESTAMPED) ||
               (dataobject->getCommand() == READ_ROW)) {
      if (dataobject->getCommand() == READ_ROW_TIMESTAMPED) {
        j[c_starttime] = dataobject->getStartTime();
        j[c_endtime] = dataobject->getEndTime();
      }
      auto arr = json::array();
      bool typemismatch = false;
      embDB_database::DbElementType type = dataobject->getDbElementType();
      for (auto& el : dataobject->getDbElements()) {
        json js;
        js[c_timestamp] = el.getTimestamp();
        switch (el.getType()) {
          case embDB_database::DbElementType::STRING:
            js[c_value] = el.toString();
            break;
          case embDB_database::DbElementType::UINT32:
            js[c_value] = el.toUint32();
            break;
          case embDB_database::DbElementType::INT32:
            js[c_value] = el.toInt32();
            break;
          case embDB_database::DbElementType::UINT64:
            js[c_value] = el.toUint64();
            break;
          case embDB_database::DbElementType::INT64:
            js[c_value] = el.toInt64();
            break;
          case embDB_database::DbElementType::FLOAT:
            js[c_value] = el.toFloat();
            break;
          case embDB_database::DbElementType::DOUBLE:
            js[c_value] = el.toDouble();
            break;
          case embDB_database::DbElementType::BOOL:
            js[c_value] = el.toBool();
            break;
          case embDB_database::DbElementType::BYTES:
            js[c_value] = el.toBytes();
            break;
          default:
            EMBDB_THROW("Internal error not known type used");
            break;
        }
        arr.push_back(js);

        // Compare actual type and requested type
        if (el.getType() != type) {
          type = el.getType();
          typemismatch = true;
        }
      }
      std::string strtype;
      if (parseDbElTypeToString(type, strtype))
        EMBDB_THROW("Internal error when parsing datatype to string");
      if (typemismatch) j[c_error] = embDB_database::DbErrorCode::TYPEMISMATCH;
      j[c_type] = strtype;
      j[c_dataarray] = arr;
    } else if ((dataobject->getCommand() != CREATE_ROW) &&
               (dataobject->getCommand() != DELETE_ROW) &&
               (dataobject->getCommand() != WRITE_ITEM)) {
      EMBDB_THROW("Internal error when evaluating command");
    }
  }

  data = j.dump();
}

//--------------------------------------------------------------------------------------------
void JsonProtocol::buildError(embDB_errorcode::ErrorCode error,
                              std::string& data) {
  json j;
  j[c_error] = error;

  data = j.dump();
}

//--------------------------------------------------------------------------------------------
bool JsonProtocol::checkFieldExists(json::object_t& field, std::string key) {
  return field.find(key) != field.end();
}

//--------------------------------------------------------------------------------------------
int JsonProtocol::parseStringToCommand(const std::string& strcommand,
                                       ProtocolCommand& protCommand) {
  if (strcommand == c_command_readcircularrow)
    protCommand = ProtocolCommand::READ_ROW;
  else if (strcommand == c_command_readcircularrowtimestamped)
    protCommand = ProtocolCommand::READ_ROW_TIMESTAMPED;
  else if (strcommand == c_command_clearallcircular)
    protCommand = ProtocolCommand::CLEAR_ALL;
  else if (strcommand == c_command_createcircularrow)
    protCommand = ProtocolCommand::CREATE_ROW;
  else if (strcommand == c_command_deletecircularrow)
    protCommand = ProtocolCommand::DELETE_ROW;
  else if (strcommand == c_command_circularrowexists)
    protCommand = ProtocolCommand::CHECK_ROW_EXISTS;
  else if (strcommand == c_command_circularrowcount)
    protCommand = ProtocolCommand::GET_ROW_COUNT;
  else if (strcommand == c_command_writeitemcircular)
    protCommand = ProtocolCommand::WRITE_ITEM;
  else
    return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
int JsonProtocol::parseCommandToString(ProtocolCommand protCommand,
                                       std::string& strcommand) {
  switch (protCommand) {
    case ProtocolCommand::READ_ROW:
      strcommand = c_command_readcircularrow;
      break;
    case ProtocolCommand::READ_ROW_TIMESTAMPED:
      strcommand = c_command_readcircularrowtimestamped;
      break;
    case ProtocolCommand::CLEAR_ALL:
      strcommand = c_command_clearallcircular;
      break;
    case ProtocolCommand::CREATE_ROW:
      strcommand = c_command_createcircularrow;
      break;
    case ProtocolCommand::DELETE_ROW:
      strcommand = c_command_deletecircularrow;
      break;
    case ProtocolCommand::CHECK_ROW_EXISTS:
      strcommand = c_command_circularrowexists;
      break;
    case ProtocolCommand::GET_ROW_COUNT:
      strcommand = c_command_circularrowcount;
      break;
    case ProtocolCommand::WRITE_ITEM:
      strcommand = c_command_writeitemcircular;
      break;
    default:
      return -1;
  }

  return 0;
}

//--------------------------------------------------------------------------------------------
int JsonProtocol::parseStringToDbElType(
    const std::string& strType, embDB_database::DbElementType& dataType) {
  if (strType == c_type_string)
    dataType = embDB_database::DbElementType::STRING;
  else if (strType == c_type_uint32)
    dataType = embDB_database::DbElementType::UINT32;
  else if (strType == c_type_int32)
    dataType = embDB_database::DbElementType::INT32;
  else if (strType == c_type_uint64)
    dataType = embDB_database::DbElementType::UINT64;
  else if (strType == c_type_int64)
    dataType = embDB_database::DbElementType::INT64;
  else if (strType == c_type_float)
    dataType = embDB_database::DbElementType::FLOAT;
  else if (strType == c_type_double)
    dataType = embDB_database::DbElementType::DOUBLE;
  else if (strType == c_type_bool)
    dataType = embDB_database::DbElementType::BOOL;
  else if (strType == c_type_bytes)
    dataType = embDB_database::DbElementType::BYTES;
  else
    return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
int JsonProtocol::parseDbElTypeToString(embDB_database::DbElementType dataType,
                                        std::string& strType) {
  switch (dataType) {
    case embDB_database::DbElementType::STRING:
      strType = c_type_string;
      break;
    case embDB_database::DbElementType::UINT32:
      strType = c_type_uint32;
      break;
    case embDB_database::DbElementType::INT32:
      strType = c_type_int32;
      break;
    case embDB_database::DbElementType::UINT64:
      strType = c_type_uint64;
      break;
    case embDB_database::DbElementType::INT64:
      strType = c_type_int64;
      break;
    case embDB_database::DbElementType::FLOAT:
      strType = c_type_float;
      break;
    case embDB_database::DbElementType::DOUBLE:
      strType = c_type_double;
      break;
    case embDB_database::DbElementType::BOOL:
      strType = c_type_bool;
      break;
    case embDB_database::DbElementType::BYTES:
      strType = c_type_bytes;
      break;
    default:
      return -1;
  }

  return 0;
}

}  // namespace embDB_protocol
