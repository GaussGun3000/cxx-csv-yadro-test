//
// Created by vva07 on 03.06.2023.
//

#include "DataFrame.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

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
    for (const auto& kvRow : m_rawData)
    {
        Row<double> newRow;
        for (const auto& kvCell : kvRow.second.rowData)
        {
            auto cell = kvCell.second;
            CellAddress cellAddress(kvCell.first, kvRow.first);
            double result;
            if(cell.at(0) == formulaFlag)
            {
                std::vector<CellAddress> callerCellVector;
                callerCellVector.push_back(cellAddress);
                result = parseFormula(std::string(std::next(cell.begin()), cell.end()), callerCellVector);
            }
            else
                try {
                    result = std::stod(cell);
                }
                catch (const std::invalid_argument& e) {
                    result = std::numeric_limits<double>::quiet_NaN();
                } catch (const std::out_of_range& e) {
                    result = std::numeric_limits<double>::quiet_NaN();
                }
            newRow.rowData.insert(std::make_pair(kvCell.first, result));
        }
        m_NumericData.insert(std::make_pair(kvRow.first, newRow));
    }
}

void DataFrame::readColNames(std::ifstream& file)
{
    std::string line;
    std::getline(file, line);
    std::vector<std::string> columnNames = splitString(line);
    m_columnNames = std::set<std::string>(std::next(columnNames.begin()), columnNames.end());
    if(m_columnNames.size() < columnNames.size() - 1)
        throw std::runtime_error("Unable to parse CSV file, duplicate column names are present!");
}

void DataFrame::readData(std::ifstream &file)
{
    std::string line;
    while(std::getline(file, line))
    if (!line.empty())
    {
        std::vector<std::string> cells = splitString(line);
        auto insertion_result = m_rowNames.insert(cells.at(0));
        if (!insertion_result.second)
            throw std::runtime_error("Unable to parse CSV file, duplicate row names are present!");
        Row<std::string> row;
        auto valueIterator = std::next(cells.begin());
        for(const auto& columnName : m_columnNames)
        {
            row.rowData.insert(std::make_pair(columnName, *valueIterator));
            valueIterator++;
        }
        this->m_rawData.insert(std::make_pair(cells.at(0), row));
    }
}

double DataFrame::parseFormula(const std::string& formula, std::vector<CellAddress>& callerCellVector)
{
    /*
    double result = 0;
    std::vector<std::string> elements = splitString(formula, "+/*-");
    if (elements.empty())
        return std::numeric_limits<double>::quiet_NaN();

    CellAddress arg1;
    CellAddress arg2;
    try
    {
        arg1 = parseCellAddress(elements.at(0));
        arg2 = parseCellAddress(elements.at(1));
    } catch(std::invalid_argument& e)
    {
        std::cout << "Invalid formula in cell " << m_columnNames.at(arg1.cNameIndex)
                  << m_rowNames.at(arg1.rNameIndex) << ". Context: \n" << e.what();
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (std::find(callerCellVector.begin(), callerCellVector.end(), arg1) != calle ||
    std::find(callerCellVector.begin(), callerCellVector.end(), arg2))
    {
        std::cout << "Invalid formula in cell " << m_columnNames.at(arg1.cNameIndex)
              << m_rowNames.at(arg1.rNameIndex)
              << ". Formula in the cell references its own cell or a circular cell reference is present!";
        return std::numeric_limits<double>::quiet_NaN();
    }
    const char op = formula.at(elements.at(0).length());
    switch (op)
    {
        case '+':
            result = m_data[arg1.rNameIndex].rowData[arg1.cNameIndex] +
                     m_data[arg2.rNameIndex].rowData[arg2.cNameIndex];
            break;
        case '-':
            result = m_data[arg1.rNameIndex].rowData[arg1.cNameIndex] -
                     m_data[arg2.rNameIndex].rowData[arg2.cNameIndex];
            break;
        case '*':
            result = m_data[arg1.rNameIndex].rowData[arg1.cNameIndex] *
                     m_data[arg2.rNameIndex].rowData[arg2.cNameIndex];
            break;
        case '/':
            if (m_data[arg2.rNameIndex].rowData[arg2.cNameIndex] == 0) {
                std::cout << "Division by zero error!";
                return std::numeric_limits<double>::quiet_NaN();
            }
            result = m_data[arg1.rNameIndex].rowData[arg1.cNameIndex] /
                     m_data[arg2.rNameIndex].rowData[arg2.cNameIndex];
            break;
        default:
            std::cout << "Unsupported operator: " << op;
            return std::numeric_limits<double>::quiet_NaN();
    } */
}

CellAddress DataFrame::parseCellAddress(const std::string& str) {
    std::string columnName;
    std::string rowName;

    size_t splitIndex = str.find_first_of("0123456789");

    if (splitIndex != std::string::npos)
    {
        columnName = str.substr(0, splitIndex);
        rowName = str.substr(splitIndex);
    }
    else
        throw std::invalid_argument("Could not parse string [" + str + "] into cellAddress!");

    if (m_columnNames.find(columnName) == m_columnNames.end() || m_rowNames.find(rowName) == m_rowNames.end() )
        throw std::invalid_argument("cell address defined by sting \"" + str + "\" was not found in DataFrame" );

    return {columnName, rowName};
}
