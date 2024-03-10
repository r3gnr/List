#include "TableClass.h"
#include <iostream>

int main() {
    Table table;

    while (true) {
        std::cout << "1 open 2 save 3 add entity 4 add attribute 5 set attribute\n 6 delete entity 7 delete attribute 8 get table 9 get entity name 10 set entity name" << std::endl;
        int choice = 0;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            std::string path;
            std::cout << "path" << std::endl;
            std::cin >> path;
            table.readTableFile(path);
        } else if (choice == 2) {
            std::string path;
            std::cout << "path" << std::endl;
            std::cin >> path;
            table.writeTableFile(path);
        } else if (choice == 3) {
            table.addRecord(std::vector<std::string>{});
            std::cout << "entity added" << std::endl;
        } else if (choice == 4) {
            std::cout << "name" << std::endl;
            std::string name;
            std::getline(std::cin, name);
            std::cout << "value" << std::endl;
            std::string value;
            std::getline(std::cin, value);
            table.addAttribute(name, value);
            std::cout << "attribute added" << std::endl;
        } else if (choice == 5) {
            size_t entityIndex = 0;
            size_t attributeIndex = 0;
            std::cout << "entityIndex" << std::endl;
            std::cin >> entityIndex;
            std::cout << "attributeIndex" << std::endl;
            std::cin >> attributeIndex;
            std::string value;
            std::cout << "value" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, value);
            table.setAttribute(entityIndex, attributeIndex, value);
            std::cout << "attribute was set" << std::endl;
        } else if (choice == 6) {
            size_t entityIndex = 0;
            std::cout << "entityIndex" << std::endl;
            std::cin >> entityIndex;
            table.deleteRecord(entityIndex);
            std::cout << "entity deleted" << std::endl;
        } else if (choice == 7) {
            size_t attributeIndex = 0;
            std::cin >> attributeIndex;
            std::cout << "attributeIndex" << std::endl;
            table.deleteAttribute(attributeIndex);
            std::cout << "attribute deleted" << std::endl;
        } else if (choice == 8) {
            std::cout << std::endl;
            std::cout << table.getEntityName() << std::endl;
            for (int i = 0; i < table.getAttributeSize(); ++i) {
                std::cout << table.getAttributeName(i) << ' ';
            }
            std::cout << std::endl;
            for (int i = 0; i < table.getRecordSize(); ++i) {
                for (int j = 0; j < table.getAttributeSize(); ++j) {
                    std::cout << table.getAttribute(i, j) << ' ';
                }
                std::cout << std::endl;
            }
        } else if (choice == 9) {
            std::cout << table.getEntityName() << std::endl;
        } else if (choice == 10) {
            std::string name;
            std::cout << "name" << std::endl;
            std::getline(std::cin, name);
            table.setEntityName(name);
        }
    }
}

