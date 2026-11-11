#include "table.h"
#include <fstream>

Table::Table(size_t recordSize, size_t attributeSize)
	: columnSize(attributeSize),
	tableMatrix(recordSize, std::vector<std::string>(attributeSize)),
	saved(recordSize == 0 && attributeSize == 0) {
	fileName = "New";
}

Table::Table(const Table& table) {
	fileName = table.fileName;
	columnSize = table.columnSize;
	rowSize = table.rowSize;
	tableMatrix = table.tableMatrix;
	tableMatrix.resize(rowSize);
	for (std::vector<std::string>& vecStr : tableMatrix) {
		vecStr.resize(columnSize);
	}
	for (int i = 0; i < rowSize; ++i) {
		for (int j = 0; j < columnSize; ++j) {
			tableMatrix[i][j] = table.tableMatrix[i][j];
		}
	}
	saved = false;
}

std::string Table::getFileName() const {
	return fileName;
}

size_t Table::getColumnSize() const {
	return columnSize;
}

size_t Table::getRowSize() const {
	return rowSize;
}

void Table::addColumn(std::string value) {
	++columnSize;
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.push_back(value);
	}
	saved = false;
}

void Table::addColumn(int value) {
	++columnSize;
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.push_back(std::to_string(value));
	}
	saved = false;
}

void Table::addRow(std::vector<std::string> entity) {
	std::vector<std::string> temp(columnSize, "");
	temp.insert(temp.end(), entity.begin(), entity.end());
	tableMatrix.push_back(temp);
	++rowSize;
	saved = false;
}
void Table::deleteColumn(size_t attributeIndex) {
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.erase(entity.begin() + attributeIndex);
	}
	--columnSize;
	saved = false;
}
void Table::deleteRow(size_t entityIndex) {
	tableMatrix.erase(tableMatrix.begin() + entityIndex);
	--rowSize;
	saved = false;
}
void Table::setCellValue(size_t entityIndex, size_t attributeIndex, std::string value) {
	tableMatrix[entityIndex][attributeIndex] = value;
	saved = false;
}

void Table::setCellValue(size_t entityIndex, size_t attributeIndex, int value) {
	tableMatrix[entityIndex][attributeIndex] = std::to_string(value);
	saved = false;
}


void Table::setRow(size_t entityIndex, std::vector<std::string> entity) {
	for (size_t i = 0; i < entity.size(); ++i) {
		tableMatrix[entityIndex][i] = entity[i];
	}
	saved = false;
}
std::vector<std::string> Table::getRow(size_t entityIndex) const {
	return tableMatrix[entityIndex];
}
std::string Table::getCellValue(size_t entityIndex, size_t attributeIndex) const {
	return tableMatrix[entityIndex][attributeIndex];
}

void Table::writeTableFile(const std::string& path) {
	std::ofstream ListFile(path);
	if (ListFile.is_open()) {
		for (const std::vector<std::string>& strs : tableMatrix) {
			for (auto it = strs.begin(); it < strs.end(); ++it) {
				if (std::next(it) == strs.end()) {
					ListFile << *it;
				} else {
					ListFile << *it << delimiter;
				}
			}
			if (strs != tableMatrix.back()) {
				ListFile << '\n';
			}
		}
		saved = true;
		ListFile.close();
	} else {
		throw std::runtime_error("Error opening a file for writing: " + path);
	}
}

void Table::readTableFile(const std::string& path, char delim) {
	delimiter = delim;
	std::ifstream ListFile(path);
	size_t sep = path.find_last_of("\\/");
	if (sep != std::string::npos) {
		fileName = path.substr(sep + 1, path.size() - 1);
	}
	if (ListFile.is_open()) {
		tableMatrix.clear();
		std::string line;
		std::vector<std::string> rowStrings;
		while (std::getline(ListFile, line)) {
			rowStrings.push_back(line);
		}
		rowSize = rowStrings.size();
		tableMatrix.resize(rowSize);
		for (size_t i = 0; i < rowSize; ++i) {
			std::string tmp = "";
			for (size_t j = 0; j < rowStrings[i].size(); ++j) {
				if (j == rowStrings[i].size() - 1 && rowStrings[i][j] != delimiter) {
					tmp.push_back(rowStrings[i][j]);
					tableMatrix[i].push_back(tmp);
					tmp = "";
				} else if (j == rowStrings[i].size() - 1 && rowStrings[i][j] == delimiter) {
					tableMatrix[i].push_back(tmp);
					tmp = "";
					tableMatrix[i].push_back(tmp);
				} else if (rowStrings[i][j] == delimiter) {
					tableMatrix[i].push_back(tmp);
					tmp = "";
				} else {
					tmp.push_back(rowStrings[i][j]);
				}
			}
		}
		size_t maxColumnSize = 0;
		for (size_t i = 0; i < rowSize; ++i) {
			if (maxColumnSize < tableMatrix[i].size()) {
				maxColumnSize = tableMatrix[i].size();
			}
		}
		columnSize = maxColumnSize;
		for (size_t i = 0; i < rowSize; ++i) {
			for (size_t j = 0; j < columnSize; ++j) {
				if (j >= tableMatrix[i].size()) {
					tableMatrix[i].push_back("");
				}
			}
		}
		saved = true;
		ListFile.close();
	} else {
		throw std::runtime_error("Error opening a file for reading: " + path);
	}
}

bool Table::isSaved() const {
	return saved;
}

void Table::clear() {
	fileName = "";
	columnSize = 0;
	rowSize = 0;
	tableMatrix.clear();
	saved = true;
}