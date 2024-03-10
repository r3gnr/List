#ifndef TABLECLASS_H
#define TABLECLASS_H

#include <string>
#include <vector>
#include <fstream>

class Table {
public:
	Table(std::string name = "", int recordSize = 0, int attributeSize = 0);
	Table(const Table& table);
	size_t getAttributeSize() const;
	size_t getRecordSize() const;
	void addAttribute(std::string name, std::string value = "");
	void addAttribute(std::string name, int value);
	void addRecord(std::vector<std::string> entity);
	void deleteAttribute(size_t attributeIndex);
	void deleteRecord(size_t entityIndex);
	void setAttribute(size_t entityIndex, size_t attributeIndex, std::string value = "");
	void setAttribute(size_t entityIndex, size_t attributeIndex, int value = 0);
	void setRecord(size_t entityIndex, std::vector<std::string> entity);
	void setEntityName(const std::string& name);
	std::vector<std::string> getRecord(size_t entityIndex) const;
	std::string	getAttribute(size_t entityIndex, size_t attributeIndex) const;
	std::string getEntityName() const;
	std::string getAttributeName(size_t attributeIndex) const;
	void writeTableFile(const std::string& path);
	void readTableFile(const std::string& path);
private:
	std::string entityName = "";
	size_t attributeSize = 0;
	size_t recordSize = 0;
	std::vector<std::string> attributeNames;
	std::vector<std::vector<std::string>> tableMatrix;
};

#endif