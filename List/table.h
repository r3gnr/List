#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

class Table {
public:
	Table(size_t recordSize = 0, size_t attributeSize = 0);
	Table(const Table& table);
	size_t getColumnSize() const;
	size_t getRowSize() const;
	void addColumn(std::string value = "");
	void addColumn(int value);
	void addRow(std::vector<std::string> record = std::vector<std::string>{});
	void deleteColumn(size_t attributeIndex);
	void deleteRow(size_t recordIndex);
	void setCellValue(size_t recordIndex, size_t attributeIndex, std::string value = "");
	void setCellValue(size_t recordIndex, size_t attributeIndex, int value = 0);
	void setRow(size_t recordIndex, std::vector<std::string> entity);
	std::vector<std::string> getRow(size_t recordIndex) const;
	std::string	getCellValue(size_t recordIndex, size_t attributeIndex) const;
	std::string getFileName() const;
	void writeTableFile(const std::string& path);
	void readTableFile(const std::string& path, char delim = ';');
	bool isSaved() const;
	void clear();
	void setDilimiter(char delim) {
		delimiter = delim;
	}
private:
	std::string fileName = "";
	size_t columnSize = 0;
	size_t rowSize = 0;
	std::vector<std::vector<std::string>> tableMatrix;
	bool saved = true;
	char delimiter = ';';
};

#endif