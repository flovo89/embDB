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

  if (getBlobInfoMutable(m_control.curindex(), &m_blobInfo))
    if (addBlobInfo(0, c_invalidIndex, &m_blobInfo)) return -1;
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
  if (!m_control.SerializeToOstream(&writer)) return DbErrorCode::INTERNAL;
  writer.flush();
  writer.close();

  // Serialize current blob to file
  if (serializeCurrentDataBlob()) return DbErrorCode::INTERNAL;

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

  elements.clear();
  uint64_t hash = m_hasher->hashStringToUint64(name);
  embDB_protolayout::BlobInfo* blobinfo;
  if (getFirstBlobInfoMutable(*m_blobInfo, &blobinfo))
    return DbErrorCode::INTERNAL;
  do {
    if (hashFoundInBlobInfo(*blobinfo, hash)) {
      if (getDataBlobMutable(blobinfo->index(), m_blobTemp))
        return DbErrorCode::INTERNAL;
      for (auto& item : m_blobTemp.blobitems())
        if (item.hash() == hash) {
          DbElement dbel;
          getDbElement(static_cast<DbElementType>(item.type()), &item.item(),
                       dbel);
          elements.push_back(dbel);
        }
    }
  } while (getNextBlobInfoMutable(*blobinfo, &blobinfo) == 1);

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
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  // Check if new blob needs to be started
  if (getSerializedBlobSize(m_blob) >=
      static_cast<uint32_t>(m_rolloverSize / c_maxBlobs))
    if (setupNextBlob()) return DbErrorCode::INTERNAL;

  // Add to current blob
  m_blobInfo->set_itemscount(m_blobInfo->itemscount() + 1);
  BlobItemLinear* blobItem = m_blob.add_blobitems();
  *blobItem->mutable_item() = element.toDataItem();
  blobItem->set_name(name);
  blobItem->set_hash(m_hasher->hashStringToUint64(name));
  blobItem->set_type(static_cast<DataType>(element.getType()));
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
int DbLayoutLinear::getFirstBlobInfoMutable(BlobInfo& reference,
                                            BlobInfo** blobinfo) {
  BlobInfo* binfo = &reference;
  while (getPrevBlobInfoMutable(*binfo, &binfo) == 1)
    ;

  if (getPrevBlobInfoMutable(*binfo, &binfo) != 0) return -1;
  *blobinfo = binfo;

  return 0;
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
  return 0;
}

//--------------------------------------------------------------------------------------------
uint32_t DbLayoutLinear::getSerializedBlobSize(const BlobLinear& blob) {
  return static_cast<uint32_t>(blob.ByteSizeLong());
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::removeDataBlob(int32_t index) {
  std::stringstream ss;
  ss << m_dataDir << "/" << c_blobPrefix << index;
  return std::remove(ss.str().c_str());
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::addBlobInfo(int32_t index, int32_t prevIndex,
                                BlobInfo** blobinfo) {
  if (index < m_control.blobinfos_size()) {
    if (getBlobInfoMutable(index, blobinfo) <= 0) return -1;
    int32_t invalidate = index + 1;
    BlobInfo* tempbinfo;
    if (invalidate == m_control.blobinfos_size()) {
      if (getBlobInfoMutable(0, &tempbinfo) <= 0) return -1;
    } else {
      if (getBlobInfoMutable(invalidate, &tempbinfo) <= 0) return -1;
    }
    tempbinfo->set_previndex(c_invalidIndex);
  } else
    *blobinfo = m_control.add_blobinfos();

  (*blobinfo)->set_index(index);
  (*blobinfo)->set_previndex(prevIndex);
  (*blobinfo)->set_nextindex(c_invalidIndex);
  (*blobinfo)->set_itemscount(0);
  (*blobinfo)->set_starttime(0);
  (*blobinfo)->set_endtime(0);

  return 0;
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::removeBlobInfo(int32_t index) {
  auto it = m_control.mutable_blobinfos()->begin();
  it += index;
  it = m_control.mutable_blobinfos()->erase(it);
}

//--------------------------------------------------------------------------------------------
bool DbLayoutLinear::hashFoundInBlobInfo(const BlobInfo& blobinfo,
                                         uint64_t hash) {
  const auto& hashes = blobinfo.hashes();
  return std::find(hashes.begin(), hashes.end(), hash) != hashes.end();
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::addKeyToBlobInfo(BlobInfo* blobinfo, std::string& key) {
  auto& hashes = blobinfo->hashes();
  uint64_t hash = m_hasher->hashStringToUint64(key);
  if (std::find(hashes.begin(), hashes.end(), hash) == hashes.end())
    blobinfo->add_hashes(hash);
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::serializeCurrentDataBlob() {
  bool ret;
  std::stringstream ss;
  ss << m_dataDir << "/" << c_blobPrefix << m_control.curindex();
  embDB_fileio::FileWriter writer(ss.str());
  writer.open();
  ret = m_control.SerializeToOstream(&writer);
  writer.flush();
  writer.close();
  return ret ? 0 : -1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::setupNextBlob() {
  // Serialize current data blob
  if (serializeCurrentDataBlob()) return -1;

  // Get next index
  int32_t curIndex = m_control.curindex();
  int32_t newIndex = curIndex + 1;
  newIndex %= c_maxBlobs;

  // Clear data-blob
  m_blob.Clear();

  // Set next blobinfo
  if (addBlobInfo(newIndex, curIndex, &m_blobInfo)) return -1;

  // Set new control index
  m_control.set_curindex(newIndex);

  return 0;
}

//--------------------------------------------------------------------------------------------
void DbLayoutLinear::getDbElement(DbElementType type, const DataItem* item,
                                  DbElement& element) {
  std::vector<uint8_t> vec;

  switch (type) {
    case DbElementType::STRING:
      element = DbElement(item->datastring(), item->timestamp());
      break;
    case DbElementType::UINT32:
      element = DbElement(item->datauint32(), item->timestamp());
      break;
    case DbElementType::INT32:
      element = DbElement(item->dataint32(), item->timestamp());
      break;
    case DbElementType::UINT64:
      element = DbElement(item->datauint64(), item->timestamp());
      break;
    case DbElementType::INT64:
      element = DbElement(item->dataint64(), item->timestamp());
      break;
    case DbElementType::FLOAT:
      element = DbElement(item->datafloat(), item->timestamp());
      break;
    case DbElementType::DOUBLE:
      element = DbElement(item->datadouble(), item->timestamp());
      break;
    case DbElementType::BOOL:
      element = DbElement(item->databool(), item->timestamp());
      break;
    case DbElementType::BYTES:
      vec =
          std::vector<uint8_t>(&item->databytes()[0],
                               &item->databytes()[item->databytes().length()]);
      element = DbElement(vec, item->timestamp());
      break;
    default:
      EMBDB_THROW("No valid type specified");
  }
}

}  // namespace embDB_database