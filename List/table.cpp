#include "table.h"

Table::Table(std::string entityName, int recordSize, int attributeSize) {
	setEntityName(this->entityName);
	saved = false;
	if (entityName == "" && recordSize == 0 && attributeSize == 0) {
		saved = true;
	}
}

Table::Table(const Table& table) {
	entityName = table.entityName;
	attributeSize = table.attributeSize;
	recordSize = table.recordSize;
	for (const std::string& str : table.attributeNames) {
		attributeNames.push_back(str);
	}
	std::vector<std::vector<std::string>> tableMatrix;
	tableMatrix.resize(recordSize);
	for (std::vector<std::string>& vecStr : tableMatrix) {
		vecStr.resize(attributeSize);
	}
	for (int i = 0; i < recordSize; ++i) {
		for (int j = 0; j < attributeSize; ++j) {
			tableMatrix[i][j] = table.tableMatrix[i][j];
		}
	}
	saved = false;
}

std::string Table::getEntityName() const {
	return entityName;
}

void Table::setEntityName(const std::string& name) {
	entityName = name;
	saved = false;
}

size_t Table::getAttributeSize() const {
	return attributeSize;
}

size_t Table::getRecordSize() const {
	return recordSize;
}

std::string Table::getAttributeName(size_t attributeIndex) const {
	return attributeNames[attributeIndex];
}

void Table::addAttribute(std::string name, std::string value) {
	++attributeSize;
	attributeNames.push_back(name);
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.push_back(value);
	}
	saved = false;
}

void Table::addAttribute(std::string name, int value) {
	++attributeSize;
	attributeNames.push_back(name);
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.push_back(std::to_string(value));
	}
	saved = false;
}

void Table::addRecord(std::vector<std::string> entity) {
	std::vector<std::string> temp(attributeSize, "");
	temp.insert(temp.end(), entity.begin(), entity.end());
	tableMatrix.push_back(temp);
	++recordSize;
	saved = false;
}
void Table::deleteAttribute(size_t attributeIndex) {
	attributeNames.erase(attributeNames.begin() + attributeIndex);
	for (std::vector<std::string>& entity : tableMatrix) {
		entity.erase(entity.begin() + attributeIndex);
	}
	--attributeSize;
	saved = false;
}
void Table::deleteRecord(size_t entityIndex) {
	tableMatrix.erase(tableMatrix.begin() + entityIndex);
	--recordSize;
	saved = false;
}
void Table::setAttribute(size_t entityIndex, size_t attributeIndex, std::string value) {
	tableMatrix[entityIndex][attributeIndex] = value;
	saved = false;
}

void Table::setAttribute(size_t entityIndex, size_t attributeIndex, int value) {
	tableMatrix[entityIndex][attributeIndex] = std::to_string(value);
	saved = false;
}


void Table::setRecord(size_t entityIndex, std::vector<std::string> entity) {
	for (size_t i = 0; i < entity.size(); ++i) {
		tableMatrix[entityIndex][i] = entity[i];
	}
	saved = false;
}
std::vector<std::string> Table::getRecord(size_t entityIndex) const {
	return tableMatrix[entityIndex];
}
std::string Table::getAttribute(size_t entityIndex, size_t attributeIndex) const {
	return tableMatrix[entityIndex][attributeIndex];
}

void Table::writeTableFile(const std::string& path) {
	std::ofstream ListFile(path);
	if (ListFile.is_open()) {
		ListFile << attributeSize << '\n' << recordSize << '\n' << entityName << '\n';
		for (const std::string& str : attributeNames) {
			ListFile << str << '\n';
		}
		for (const std::vector<std::string>& strs : tableMatrix) {
			for (const std::string& str : strs) {
				ListFile << str << '\n';
			}
		}
		saved = true;
		ListFile.close();
	} else {
		throw std::runtime_error("Error opening a file for writing: " + path);
	}
}

void Table::readTableFile(const std::string& path) {
	std::ifstream ListFile(path);
	if (ListFile.is_open()) {
		attributeNames.clear();
		tableMatrix.clear();
		std::string line;
		std::getline(ListFile, line);
		attributeSize = std::stoul(line);
		std::getline(ListFile, line);
		recordSize = std::stoul(line);
		std::getline(ListFile, line);
		entityName = line;
		std::string attribute;
		for (size_t i = 0; i < attributeSize; ++i) {
			std::getline(ListFile, line);
			attributeNames.push_back(line);
		}
		tableMatrix.resize(recordSize);
		for (size_t i = 0; i < recordSize; ++i) {
			tableMatrix[i].resize(attributeSize);
		}
		for (size_t i = 0; i < recordSize; ++i) {
			for (size_t j = 0; j < attributeSize; ++j) {
				std::getline(ListFile, line);
				tableMatrix[i][j] = line;
			}
		}
		saved = true;
		ListFile.close();
	} else {
		throw std::runtime_error("Error opening a file for reading: " + path);
	}
}

void Table::renameAttribute(size_t attributeIndex, const std::string& name) {
	attributeNames[attributeIndex] = name;
	saved = false;
}

bool Table::isSaved() const {
	return saved;
}