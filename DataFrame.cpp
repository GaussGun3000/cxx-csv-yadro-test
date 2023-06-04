//
// Created by vva07 on 03.06.2023.
//

#include "DataFrame.h"
#include <iostream>
#include <fstream>
#include <limits>

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
    if (m_rawData.empty())
        throw std::runtime_error("Raw data of DataFrame is empty. Check CSV file");

    const char formulaFlag = '=';
    for (const auto& row : m_rawData)
    {
        Row<double> newRow;
        for (const auto& cell : row.rowData)
        {
            double result;
            if(cell.at(0) == formulaFlag)
                result = parseFormula(std::string(std::next(cell.begin()), cell.end()));
            else
                try {
                    result = std::stod(cell);
                }
                catch (const std::invalid_argument& e) {
                    result = std::numeric_limits<double>::quiet_NaN();
                } catch (const std::out_of_range& e) {
                    result = std::numeric_limits<double>::quiet_NaN();
                }
            newRow.rowData.push_back(result);
        }
    }
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

double DataFrame::parseFormula(const std::string& formula)
{
    double result = 0;
    std::vector<std::string> elements = splitString(formula, "+/*-");
    if(elements.empty())
        return  std::numeric_limits<double>::quiet_NaN();

    auto value_1
}

