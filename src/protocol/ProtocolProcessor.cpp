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

#include "ProtocolProcessor.hpp"

#include "../database/DbElementType.hpp"
#include "../database/IDataBase.hpp"
#include "../exception/Exception.hpp"
#include "IProtocol.hpp"

namespace embDB_protocol {

//--------------------------------------------------------------------------------------------
ProtocolProcessor::ProtocolProcessor(std::unique_ptr<IProtocolArbiter> arbiter,
                                     embDB_database::DbGuard& guard)
    : m_arbiter(std::move(arbiter)), m_guard(guard) {}

//--------------------------------------------------------------------------------------------
ProtocolProcessor::~ProtocolProcessor() {}

//--------------------------------------------------------------------------------------------
void ProtocolProcessor::notifier(embDB_server::ISocketClient& client) {
  std::shared_ptr<IProtocol> protocol;
  std::unique_ptr<DataObject> dataobj;
  ProtErrorCode error;
  std::string output;

  while (m_arbiter->getProtocol(client.getClientBuffer(), protocol)) {
    error = protocol->translateToDataObject(dataobj);

    if (error != embDB_errorcode::ERROR_SUCCESS) {
      protocol->buildError(error, output);
    } else {
      processData(*dataobj);
      protocol->translateToString(dataobj, output);
    }

    client.writeString(output);
  }
}

//--------------------------------------------------------------------------------------------
void ProtocolProcessor::processData(DataObject& obj) {
  switch (obj.getCommand()) {
    case ProtocolCommand::READ_ROW: {
      std::list<embDB_database::DbElement> elements;
      obj.setErrorCode(m_guard.getAllItems(obj.getName(), elements));
      obj.setDbElements(elements);
      break;
    }
    case ProtocolCommand::READ_ROW_TIMESTAMPED: {
      std::list<embDB_database::DbElement> elements;
      obj.setErrorCode(m_guard.getItemsBetween(
          obj.getName(), obj.getStartTime(), obj.getEndTime(), elements));
      obj.setDbElements(elements);
      break;
    }
    case ProtocolCommand::CLEAR_ALL: {
      obj.setErrorCode(m_guard.clearAll());
      break;
    }
    case ProtocolCommand::CREATE_ROW: {
      obj.setErrorCode(m_guard.createRow(obj.getName(), obj.getDbElementType(),
                                         obj.getItemsCount()));
      break;
    }
    case ProtocolCommand::DELETE_ROW: {
      obj.setErrorCode(m_guard.deleteRow(obj.getName()));
      break;
    }
    case ProtocolCommand::CHECK_ROW_EXISTS: {
      bool exists;
      obj.setErrorCode(m_guard.rowExists(obj.getName(), exists));
      obj.setRowExists(exists);
      break;
    }
    case ProtocolCommand::GET_ROW_COUNT: {
      uint32_t count;
      obj.setErrorCode(m_guard.getRowCount(count));
      obj.setRowCount(count);
      break;
    }
    case ProtocolCommand::WRITE_ITEM: {
      obj.setErrorCode(m_guard.addItem(obj.getName(), obj.getDbElement()));
      break;
    }
    default: {
      EMBDB_THROW("Unknown command");
    }
  }
}

}  // namespace embDB_protocol
