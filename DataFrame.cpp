//
// Created by vva07 on 03.06.2023.
//

#include "DataFrame.h"
#include <iostream>
#include <fstream>

DataFrame::DataFrame(const std::string &filePath, const char delimiter)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "WARNING: COULD NOT OPEN THE CSV FILE (" << filePath << ")" << std::endl;
        throw std::runtime_error("Failed to open CSV file: " + filePath);
    }

    readColNames(file);
    readData(file);
}

DataFrame::DataFrame(const std::string &filePath, const uint32_t rcNamingMode, const char delimiter)
{

}


std::vector<std::string> DataFrame::splitString(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    std::string remainingStr = str;
    size_t pos = 0;
    while ((pos = remainingStr.find_first_of(delimiters)) != std::string::npos) {
        std::string token = remainingStr.substr(0, pos);
        tokens.push_back(token);
        remainingStr = remainingStr.substr(pos + 1);
    }
    if (!remainingStr.empty()) {
        tokens.push_back(remainingStr);
    }
    return tokens;
}

void DataFrame::printData(uint32_t rows, uint32_t columns)
{

}

void DataFrame::translateToNumeric()
{

}

void DataFrame::readColNames(std::ifstream& file)
{
    std::string line;
    std::getline(file, line);
    std::vector<std::string> columnNames = splitString(line);
    this->m_columnNames = std::vector<std::string>(std::next(columnNames.begin()), columnNames.end());
}

void DataFrame::readData(std::ifstream &file)
{
    std::string line;
    while(std::getline(file, line))
    if (!line.empty())
    {
        std::vector<std::string> cells = splitString(line);
        this->m_rowNames.push_back(cells.at(0));
        Row<std::string> row;
        row.rowData.insert(row.rowData.end(),
                           std::make_move_iterator(std::next(cells.begin())),
                           std::make_move_iterator(cells.end()));
        this->m_rawData.push_back(row);
    }
}

