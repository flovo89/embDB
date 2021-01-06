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
#ifndef _EMBDB_EVENT_LOOP_FACTORY_HPP_
#define _EMBDB_EVENT_LOOP_FACTORY_HPP_

#include "IEventLoop.hpp"

namespace embDB_eventloop {

class EventLoopFactory {
 public:
  EventLoopFactory() = delete;
  ~EventLoopFactory() = delete;
  EventLoopFactory(EventLoopFactory const&) = delete;
  EventLoopFactory(EventLoopFactory&&) = delete;
  EventLoopFactory& operator=(EventLoopFactory const&) = delete;
  EventLoopFactory& operator=(EventLoopFactory&&) = delete;

  static int createDefaultEventLoop(std::unique_ptr<IEventLoop>& el);
  static int createlibeventEventLoop(std::unique_ptr<IEventLoop>& el);
};

}  // namespace embDB_eventloop
#endif
