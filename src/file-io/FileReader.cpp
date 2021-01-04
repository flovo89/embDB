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
#include "FileReader.hpp"

namespace embDB_fileio {

//--------------------------------------------------------------------------------------------
FileReader::FileReader(std::string path) : m_path(path) {}

//--------------------------------------------------------------------------------------------
FileReader::~FileReader() {}

//--------------------------------------------------------------------------------------------
void FileReader::open() {
  std::fstream::open(m_path.c_str(), std::ios::in | std::ios::binary);
}

//--------------------------------------------------------------------------------------------
void FileReader::close() { std::fstream::close(); }

}  // namespace embDB_fileio
