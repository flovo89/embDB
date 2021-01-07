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

#ifndef _EMBDB_PROTOCOL_ARBITER_HPP_
#define _EMBDB_PROTOCOL_ARBITER_HPP_

#include <list>
#include <memory>
#include <sstream>

#include "IProtocolArbiter.hpp"

namespace embDB_protocol {

class ProtocolArbiter : public IProtocolArbiter {
 public:
  ProtocolArbiter(std::list<std::shared_ptr<IProtocol>>& protocols);
  virtual ~ProtocolArbiter();

  // IProtocolArbiter
  virtual bool getProtocol(embDB_server::ClientBuffer& buffer,
                           std::shared_ptr<IProtocol>& protocol) override;

 private:
  std::list<std::shared_ptr<IProtocol>>& m_protocols;
};

}  // namespace embDB_protocol
#endif
