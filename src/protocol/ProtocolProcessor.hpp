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

#ifndef _EMBDB_PROTOCOL_PROCESSOR_HPP_
#define _EMBDB_PROTOCOL_PROCESSOR_HPP_

#include <memory>
#include <sstream>

#include "../database/db-guard/DbGuard.hpp"
#include "../server/IClientReception.hpp"
#include "IProtocolArbiter.hpp"

namespace embDB_protocol {

class ProtocolProcessor : public embDB_server::IClientReception {
 public:
  ProtocolProcessor(std::unique_ptr<IProtocolArbiter> arbiter,
                    embDB_database::DbGuard& guard);
  virtual ~ProtocolProcessor();

  // embDB_server::IClientReception
  virtual void notifier(embDB_server::ISocketClient& client) override;

 private:
  std::unique_ptr<IProtocolArbiter> m_arbiter;
  embDB_database::DbGuard& m_guard;

  void processData(DataObject& obj);
};

}  // namespace embDB_protocol
#endif
