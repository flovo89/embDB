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

#include "DbLayoutCircular.hpp"

#include "../../exception/Exception.hpp"

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbLayoutCircular::DbLayoutCircular(
    std::unique_ptr<embDB_fileio::FileReader> reader,
    std::unique_ptr<embDB_fileio::FileWriter> writer,
    std::unique_ptr<embDB_utilities::IHasher> hasher,
    std::unique_ptr<embDB_utilities::ITimestamper> timestamper)
    : m_reader(std::move(reader)),
      m_writer(std::move(writer)),
      m_hasher(std::move(hasher)),
      m_timestamper(std::move(timestamper)),
      m_isDeserialized(false) {}

//--------------------------------------------------------------------------------------------
DbLayoutCircular::~DbLayoutCircular() {}

//--------------------------------------------------------------------------------------------
int DbLayoutCircular::init() {
  // Deserialize from file
  m_reader->open();
  if (!m_dataLayout.ParseFromIstream(m_reader.get())) {
    return -1;
  }
  m_reader->close();

  m_isDeserialized = true;

  uint32_t version;
  getVersionCircular(version);

  return 0;
}

//--------------------------------------------------------------------------------------------
int DbLayoutCircular::deinit() {
  if (serialize() != DbErrorCode::SUCCESS) return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::serialize() {
  // Set version
  m_dataLayout.mutable_header()->set_version(c_version);

  // Serialize to file
  m_writer->open();
  if (!m_dataLayout.SerializeToOstream(m_writer.get())) {
    return DbErrorCode::INTERNAL;
  }
  m_writer->flush();
  m_writer->close();

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::clearAll() {
  m_dataLayout.Clear();

  // Jap, because we start from scratch
  m_isDeserialized = true;

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::getVersionCircular(uint32_t& version) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  version = m_dataLayout.header().version();

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::getRowCount(uint32_t& count) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  count = static_cast<uint32_t>(m_dataLayout.rows_size());
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::createRow(std::string name, DbElementType type,
                                        uint32_t maxItems) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  DataRowCircular stub;
  uint32_t stubitems;
  DbElementType stubtype;
  if (!getRow(name, stub)) {
    getMaxItems(stub, stubitems);
    getType(stub, stubtype);
    if (maxItems != stubitems)
      return DbErrorCode::ITEMSCOUNTMISMATCH;
    else if (type != stubtype)
      return DbErrorCode::TYPEMISMATCH;
    else
      return DbErrorCode::SUCCESS;
  }

  DataRowCircular* row = m_dataLayout.add_rows();
  uint64_t hash = m_hasher->hashStringToUint64(name);
  row->set_name(name);
  row->set_hash(hash);
  row->set_type(static_cast<DataType>(type));
  row->set_maxitems(maxItems);
  row->set_curitem(0);
  row->set_overflow(false);

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::rowExists(std::string name, bool& exists) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  exists = false;

  DataRowCircular row;
  if (getRow(name, row)) return DbErrorCode::SUCCESS;

  // If this occurs, two strings might have generated same hash...
  std::string rowName;
  getName(row, rowName);
  if (name != rowName) return DbErrorCode::HASHNAMEMISMATCH;

  exists = true;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::deleteRow(std::string name) {
  if (!m_isDeserialized) {
    return DbErrorCode::INTERNAL;
  }

  uint64_t hash = m_hasher->hashStringToUint64(name);
  int index = 0;

  if (m_dataLayout.rows_size() > 0) {
    for (auto& r : m_dataLayout.rows()) {
      if (r.hash() == hash) {
        m_dataLayout.mutable_rows()->DeleteSubrange(index, 1);
        return DbErrorCode::SUCCESS;
      }
      index++;
    }
  }

  return DbErrorCode::NOTFOUND;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::getAllItemsCircular(
    std::string name, std::list<DbElement>& elements) {
  RepeatedPtrField<DataItem> dataItems;
  DataRowCircular row;
  bool overflow;
  uint32_t maxItems;
  uint32_t curItem;
  DbElementType readtype;
  DbElement element;

  if (getRow(name, row)) return DbErrorCode::NOTFOUND;

  getType(row, readtype);

  getMaxItems(row, maxItems);

  getCurItem(row, curItem);

  getOverflow(row, overflow);

  getDataItems(row, dataItems);

  elements.clear();
  if (dataItems.size() > 0) {
    if (overflow) {
      for (uint32_t i = curItem; i < maxItems; i++) {
        getDataElement(readtype, &dataItems.Get(i), element);
        elements.push_back(element);
      }
    }
    for (uint32_t i = 0; i < curItem; i++) {
      getDataElement(readtype, &dataItems.Get(i), element);
      elements.push_back(element);
    }
  }

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::getItemsBetweenCircular(
    std::string name, int64_t start, int64_t end,
    std::list<DbElement>& elements) {
  std::list<DbElement> tempElements;
  DbErrorCode err = getAllItemsCircular(name, tempElements);

  if (err != DbErrorCode::SUCCESS) return err;

  // Be aware that a wrong timebase leads lost order of data...
  elements.clear();
  for (auto& i : tempElements)
    if ((i.getTimestamp() >= start) && (i.getTimestamp() <= end))
      elements.push_back(i);

  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutCircular::addItemCircular(std::string name,
                                              const DbElement& element) {
  RepeatedPtrField<DataItem>* dataItems;
  DataRowCircular* row;
  bool overflow;
  uint32_t maxItems;
  uint32_t curItem;
  DbElementType readtype;

  if (getRowMutable(name, &row)) return DbErrorCode::NOTFOUND;

  getType(*row, readtype);
  if (readtype != element.getType()) return DbErrorCode::TYPEMISMATCH;

  getMaxItems(*row, maxItems);

  getCurItem(*row, curItem);

  getOverflow(*row, overflow);

  getDataItemsMutable(row, &dataItems);

  if (curItem >= maxItems) return DbErrorCode::INTERNAL;

  if (dataItems->empty() && overflow) return DbErrorCode::INTERNAL;

  DataItem* it;
  if (!overflow) {
    it = dataItems->Add();
  } else {
    it = dataItems->Mutable(curItem);
  }

  it->set_timestamp(m_timestamper->getTimestampMilliseconds());
  setDataItem(element, it);

  curItem++;

  if (curItem == maxItems) {
    overflow |= true;
    curItem = 0;
  }

  setCurItem(row, curItem);
  setOverflow(row, overflow);

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
int DbLayoutCircular::getRow(std::string name, DataRowCircular& row) const {
  uint64_t hash = m_hasher->hashStringToUint64(name);

  if (m_dataLayout.rows_size() > 0) {
    for (auto& r : m_dataLayout.rows()) {
      if (r.hash() == hash) {
        row = r;
        return 0;
      }
    }
  }

  return -1;
}

//--------------------------------------------------------------------------------------------
int DbLayoutCircular::getRowMutable(std::string name, DataRowCircular** row) {
  uint64_t hash = m_hasher->hashStringToUint64(name);
  bool found = false;
  uint32_t index = 0;

  if (m_dataLayout.rows_size() > 0) {
    for (auto& r : m_dataLayout.rows()) {
      if (r.hash() == hash) {
        found = true;
        break;
      }
      index++;
    }
  }

  if (found) {
    *row = m_dataLayout.mutable_rows(index);
    return 0;
  }

  return -1;
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getName(const DataRowCircular& row,
                               std::string& name) const {
  name = row.name();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getType(const DataRowCircular& row,
                               DbElementType& type) const {
  type = static_cast<DbElementType>(row.type());
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getOverflow(const DataRowCircular& row,
                                   bool& overflow) const {
  overflow = row.overflow();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::setOverflow(DataRowCircular* row, bool overflow) {
  row->set_overflow(overflow);
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getMaxItems(const DataRowCircular& row,
                                   uint32_t& maxItems) const {
  maxItems = row.maxitems();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getCurItem(const DataRowCircular& row,
                                  uint32_t& curItem) const {
  curItem = row.curitem();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::setCurItem(DataRowCircular* row, uint32_t curItem) {
  row->set_curitem(curItem);
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getDataItems(const DataRowCircular& row,
                                    RepeatedPtrField<DataItem>& items) const {
  items = row.items();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getDataItemsMutable(DataRowCircular* row,
                                           RepeatedPtrField<DataItem>** items) {
  *items = row->mutable_items();
}

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::getDataElement(DbElementType type, const DataItem* item,
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

//--------------------------------------------------------------------------------------------
void DbLayoutCircular::setDataItem(const DbElement& element, DataItem* item) {
  std::vector<uint8_t> vec;

  switch (element.getType()) {
    case DbElementType::STRING:
      item->set_datastring(element.toString());
      break;
    case DbElementType::UINT32:
      item->set_datauint32(element.toUint32());
      break;
    case DbElementType::INT32:
      item->set_dataint32(element.toInt32());
      break;
    case DbElementType::UINT64:
      item->set_datauint64(element.toUint64());
      break;
    case DbElementType::INT64:
      item->set_dataint64(element.toInt64());
      break;
    case DbElementType::FLOAT:
      item->set_datafloat(element.toFloat());
      break;
    case DbElementType::DOUBLE:
      item->set_datadouble(element.toDouble());
      break;
    case DbElementType::BOOL:
      item->set_databool(element.toBool());
      break;
    case DbElementType::BYTES:
      vec = element.toBytes();
      item->set_databytes(std::string(vec.begin(), vec.end()));
      break;
    default:
      EMBDB_THROW("No valid type specified");
  }
}

}  // namespace embDB_database
