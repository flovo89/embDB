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

#include <sstream>

#include "../../exception/Exception.hpp"
#include "../../file-io/FileReader.hpp"
#include "../../file-io/FileWriter.hpp"

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbLayoutLinear::DbLayoutLinear(
    std::unique_ptr<embDB_utilities::IHasher> hasher,
    std::unique_ptr<embDB_utilities::ITimestamper> timestamper,
    std::string linearDir, uint32_t rolloverSize)
    : m_hasher(std::move(hasher)),
      m_timestamper(std::move(timestamper)),
      m_dataDir(linearDir),
      m_rolloverSize((uint64_t)rolloverSize * 1024 * 1024),
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

  if (!getBlobInfoMutable(m_control.curindex(), &m_blobInfo))
    addBlobInfo(0, c_invalidIndex, &m_blobInfo);
  getDataBlobMutable(m_blobInfo->index(), m_blob);

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

  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  // TODO: Check if place --> otherwise new blob

  // Add to current blob
  m_blobInfo->set_itemscount(m_blobInfo->itemscount() + 1);
  BlobItemLinear* blobItem = m_blob.add_blobitems();
  *blobItem->mutable_item() = element.toDataItem();
  blobItem->set_name(name);
  addKeyToBlobInfo(m_blobInfo, name);

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getBlobInfoMutable(int32_t index, BlobInfo** blobinfo) {
  if (m_control.blobinfos_size() == 0) return 0;
  if (index >= m_control.blobinfos_size()) return -1;
  *blobinfo = m_control.mutable_blobinfos(index);
  return 1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getNextBlobInfoMutable(const BlobInfo& reference,
                                           BlobInfo** blobinfo) {
  if (reference.nextindex() >= m_control.blobinfos_size()) return -1;
  if (reference.nextindex() == c_invalidIndex) return 0;
  *blobinfo = m_control.mutable_blobinfos(reference.nextindex());
  return 1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getPrevBlobInfoMutable(const BlobInfo& reference,
                                           BlobInfo** blobinfo) {
  if (reference.previndex() >= m_control.blobinfos_size()) return -1;
  if (reference.previndex() == c_invalidIndex) return 0;
  *blobinfo = m_control.mutable_blobinfos(reference.previndex());
  return 1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::getDataBlobMutable(int32_t index, BlobLinear& blob) {
  std::stringstream ss;
  ss << m_dataDir << "/" << c_blobPrefix << index;
  embDB_fileio::FileReader reader(ss.str());
  reader.open();
  if (!blob.ParseFromIstream(&reader)) {
    return -1;
  }
  reader.close();
  return 1;
}

//--------------------------------------------------------------------------------------------
uint32_t DbLayoutLinear::getSerializedBlobSize(const BlobLinear& blob) {
  return static_cast<uint32_t>(blob.ByteSizeLong());
}

//--------------------------------------------------------------------------------------------
int removeDataBlob(int32_t index) {
  (void)index;
  return -1;
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::addBlobInfo(int32_t index, int32_t prevIndex,
                                 BlobInfo** blobinfo) {
  *blobinfo = m_control.add_blobinfos();
  (*blobinfo)->set_index(index);
  (*blobinfo)->set_previndex(prevIndex);
  (*blobinfo)->set_nextindex(c_invalidIndex);
  (*blobinfo)->set_itemscount(0);
  (*blobinfo)->set_serializedsize(0);
  (*blobinfo)->set_starttime(0);
  (*blobinfo)->set_endtime(0);
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::removeBlobInfo(int32_t index) {
  auto it = m_control.mutable_blobinfos()->begin();
  it += index;
  it = m_control.mutable_blobinfos()->erase(it);
}

//--------------------------------------------------------------------------------------------
bool DbLayoutLinear::keyFoundInBlobInfo(const BlobInfo& blobinfo,
                                        std::string& key) {
  const auto& hashes = blobinfo.hashes();
  uint64_t hash = m_hasher->hashStringToUint64(key);
  return std::find(hashes.begin(), hashes.end(), hash) != hashes.end();
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::addKeyToBlobInfo(BlobInfo* blobinfo, std::string& key) {
  auto& hashes = blobinfo->hashes();
  uint64_t hash = m_hasher->hashStringToUint64(key);
  if (std::find(hashes.begin(), hashes.end(), hash) == hashes.end())
    blobinfo->add_hashes(hash);
}

}  // namespace embDB_database