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
#ifndef _EMBDB_DB_LAYOUT_LINEAR_HPP_
#define _EMBDB_DB_LAYOUT_LINEAR_HPP_

#include <memory>

#include "../../../embdb-layout/cpp/embdb-layout.pb.h"
#include "../../utilities/IHasher.hpp"
#include "../../utilities/ITimestamper.hpp"
#include "../IDataBaseLinear.hpp"

namespace embDB_database {

using namespace google::protobuf;
using namespace embDB_protolayout;

class DbLayoutLinear : public IDataBaseLinear {
 public:
  DbLayoutLinear(std::unique_ptr<embDB_utilities::IHasher> hasher,
                 std::unique_ptr<embDB_utilities::ITimestamper> timestamper,
                 std::string linearDir);
  virtual ~DbLayoutLinear();

  friend class DbGuard;

 private:
  // IDataBase
  virtual int init() override;
  virtual int deinit() override;
  virtual DbErrorCode serialize() override;
  virtual DbErrorCode clearAll() override;

  // IDataBaseLinear
  virtual DbErrorCode getVersionLinear(uint32_t& version) override;

  virtual DbErrorCode getAllItemsLinear(
      std::string name, std::list<DbElement>& elements) override;
  virtual DbErrorCode getItemsBetweenLinear(
      std::string name, int64_t start, int64_t end,
      std::list<DbElement>& elements) override;
  virtual DbErrorCode addItemLinear(std::string name,
                                    const DbElement& element) override;

  std::unique_ptr<embDB_utilities::IHasher> m_hasher;
  std::unique_ptr<embDB_utilities::ITimestamper> m_timestamper;
  std::string m_dataDir;

  bool m_isDeserialized;
  embDB_protolayout::ControlLinear m_control;

  const uint32_t c_version = 1;
  const std::string c_controlFileName = "embDB_control";
  const int32_t c_invalidIndex = -1;

  int getBlobInfo(uint32_t index, BlobInfo& blobinfo);
  int getNextBlobInfo(const BlobInfo& reference, BlobInfo& blobinfo);
  int getPrevBlobInfo(const BlobInfo& reference, BlobInfo& blobinfo);
  int getDataBlob(uint32_t index, BlobLinear& blob);
};

}  // namespace embDB_database
#endif
