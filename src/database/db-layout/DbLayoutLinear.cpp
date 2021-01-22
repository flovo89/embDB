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

#include "DbLayoutLinear.hpp"

#include "../../exception/Exception.hpp"
#include "../../file-io/FileReader.hpp"
#include "../../file-io/FileWriter.hpp"

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbLayoutLinear::DbLayoutLinear(
    std::unique_ptr<embDB_utilities::IHasher> hasher,
    std::unique_ptr<embDB_utilities::ITimestamper> timestamper,
    std::string linearDir)
    : m_hasher(std::move(hasher)),
      m_timestamper(std::move(timestamper)),
      m_dataDir(linearDir),
      m_isDeserialized(false) {}

//--------------------------------------------------------------------------------------------
DbLayoutLinear::~DbLayoutLinear() {}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::init() {
  // Deserialize from file
  embDB_fileio::FileReader reader(m_dataDir + "/" + c_controlFileName);
  reader.open();
  if (!m_control.ParseFromIstream(&reader)) {
    return -1;
  }
  reader.close();

  m_isDeserialized = true;
  return 0;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::deinit() {
  if (serialize() != DbErrorCode::SUCCESS) return -1;
  return 0;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::serialize() {
  // Set version
  m_control.mutable_header()->set_version(c_version);

  // Serialize control to file
  embDB_fileio::FileWriter writer(m_dataDir + "/" + c_controlFileName);
  writer.open();
  if (!m_control.SerializeToOstream(&writer)) {
    return DbErrorCode::INTERNAL;
  }
  writer.flush();
  writer.close();

  // TODO: Serialize current blob to file

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::clearAll() { return DbErrorCode::SUCCESS; }

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getVersionLinear(uint32_t& version) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  version = m_control.header().version();

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getAllItemsLinear(std::string name,
                                              std::list<DbElement>& elements) {
  (void)name;
  (void)elements;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getItemsBetweenLinear(
    std::string name, int64_t start, int64_t end,
    std::list<DbElement>& elements) {
  (void)name;
  (void)start;
  (void)end;
  (void)elements;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::addItemLinear(std::string name,
                                          const DbElement& element) {
  (void)name;
  (void)element;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getBlobInfo(uint32_t index, BlobInfo& blobinfo) {
  if (index >= (uint32_t)m_control.blobinfos_size()) return -1;
  blobinfo = m_control.blobinfos()[index];

  if (m_control.blobinfos_size() > 1)
    if (blobinfo.previndex() == c_invalidIndex) return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getNextBlobInfo(const BlobInfo& reference,
                                    BlobInfo& blobinfo) {
  (void)reference;
  (void)blobinfo;
  return -1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getPrevBlobInfo(const BlobInfo& reference,
                                    BlobInfo& blobinfo) {
  (void)reference;
  (void)blobinfo;
  return -1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getDataBlob(uint32_t index, BlobLinear& blob) {
  (void)index;
  (void)blob;
  return -1;
}

}  // namespace embDB_database