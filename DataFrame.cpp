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
        throw std::runtime_error("Failed to open CSV file: " + filePath);
    }

    readColNames(file, delimiter);
    readData(file, delimiter);
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
    if (m_NumericData.empty())
    {
        std::cout << "Error: Numeric data of DataFrame is not filled!\n";
        return;
    }
    if (rows == 0)
        rows = m_rowNames.size();
    if (columns == 0)
        columns = m_columnNames.size();

    uint32_t rowCounter = 0, columnCounter = 0;
    std::cout.precision(3);
    std::cout << "\n\t\t--- DataFrame ---\n\n";
    for (const auto& cName : m_columnNames)
    {
        if (columnCounter >= columns)
            break;
        columnCounter == 0 ? std::cout << "    " : std::cout << ",";
        std::cout << " " << cName;
        columnCounter++;
    }
    for (const auto& rName : m_rowNames)
    {
        if (rowCounter >= rows) break;
        std::cout << std::endl << rName << "  ";
        columnCounter = 0;
        for (const auto& cName: m_columnNames)
        {
            if (columnCounter >= columns) break;
            if (columnCounter > 0) std::cout << ",";
            std::cout << m_NumericData.at(rName).rowData.at(cName);
            columnCounter++;
        }
        rowCounter++;
    }

}

void DataFrame::readColNames(std::ifstream& file, const char delimiter)
{
    std::string line;
    std::getline(file, line);
    std::vector<std::string> columnNames = splitString(line, std::string(1, delimiter));
    m_columnNames = std::set<std::string, StringWeightComparator>(std::next(columnNames.begin()), columnNames.end());
    m_columnNamesOriginalOrder.insert(m_columnNamesOriginalOrder.end(),
                                      std::make_move_iterator(std::next(columnNames.begin())),
                                      std::make_move_iterator(columnNames.end()));
    if(m_columnNames.size() < columnNames.size() - 1)
        throw std::runtime_error("Unable to parse CSV file, duplicate column names are present!");
}

void DataFrame::readData(std::ifstream &file, const char delimiter)
{
    std::string line;
    while(std::getline(file, line))
    if (!line.empty())
    {
        std::vector<std::string> cells = splitString(line, std::string(1, delimiter));
        if (cells.size() != m_columnNames.size() + 1)
            throw std::out_of_range("Not enough columns in row " + cells.at(0));
        auto insertion_result = m_rowNames.insert(cells.at(0));
        if (!insertion_result.second)
            throw std::runtime_error("Unable to parse CSV file, duplicate row names are present!");
        Row<std::string> row;
        auto valueIterator = std::next(cells.begin());
        for(const auto& columnName : m_columnNamesOriginalOrder)
        {
            row.rowData.insert(std::make_pair(columnName, *valueIterator));
            valueIterator++;
        }
        this->m_rawData.insert(std::make_pair(cells.at(0), row));
    }
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
                std::set<CellAddress> callerCellSet;
                callerCellSet.insert(cellAddress);
                result = parseFormula(std::string(std::next(cell.begin()), cell.end()), cellAddress, cellAddress,
                                      callerCellSet);
            }
            else
                result = parseDouble(cellAddress);
            newRow.rowData.insert(std::make_pair(kvCell.first, result));
        }
        m_NumericData.insert(std::make_pair(kvRow.first, newRow));
    }
}

double DataFrame::parseFormula(const std::string &formula, CellAddress &currentCell, CellAddress &originalCaller,
                               std::set<CellAddress> &callingCells)
{
    double result = std::numeric_limits<double>::quiet_NaN();
    std::vector<std::string> elements = splitString(formula, "+/*-");
    if (elements.empty())
        return std::numeric_limits<double>::quiet_NaN();

    CellAddress caArg1;
    CellAddress caArg2;
    try
    {
        caArg1 = parseCellAddress(elements.at(0));
        caArg2 = parseCellAddress(elements.at(1));
    } catch(std::invalid_argument& e)
    {
        if(m_enableWarnings) std::cout << "Warning: Invalid formula in cell " << originalCaller.cName
                  << originalCaller.rName << ".\nContext: " << e.what() << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto it1 = callingCells.find(caArg1);
    auto it2 = callingCells.find(caArg2);
    if (it1 != callingCells.end() || it2 != callingCells.end())
    {
        if(m_enableWarnings) std::cout << "Warning: Invalid formula in cell " << originalCaller.cName << originalCaller.rName
        << ". Formula in the cell references its own cell or a circular cell reference is present!" << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    const char op = formula.at(elements.at(0).length());
    std::string arg1Raw = findCell(caArg1);
    std::string arg2Raw = findCell(caArg2);
    double arg1value;
    double arg2value;
    if (arg1Raw.at(0) == '=')
    {
        callingCells.insert(currentCell);
        arg1value = parseFormula(std::string(std::next(arg1Raw.begin()), arg1Raw.end()), caArg1, originalCaller,
                                 callingCells);
    }
    else arg1value = parseDouble(caArg1);
    if (arg2Raw.at(0) == '=')
    {
        callingCells.insert(currentCell);
        arg2value = parseFormula(std::string(std::next(arg2Raw.begin()), arg2Raw.end()), caArg2, originalCaller,
                                 callingCells);
    }
    else arg2value = parseDouble(caArg2);
    try
    {
        result = arithmeticOperation(arg1value, arg2value, op);
    }catch (std::invalid_argument& e)
    {
        if(m_enableWarnings) std::cout << "Warning: Failed to calculate value at " << currentCell.cName << currentCell.rName <<
        ".\nContext: " << e.what() << std::endl;
    }
    callingCells.erase(currentCell);
    return result;
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

std::string DataFrame::findCell(CellAddress &ca)
{
    std::string str = m_rawData.at(ca.rName).rowData.at(ca.cName);
    return std::move(str);
}

double DataFrame::arithmeticOperation(double arg1, double arg2, const char op)
{
    double result = std::numeric_limits<double>::quiet_NaN();
    switch (op)
    {
        case '+':
            result = arg1 + arg2;
            break;
        case '-':
            result = arg1 - arg2;
            break;
        case '*':
            result = arg1 * arg2;
            break;
        case '/':
            if (arg2 == 0)
                throw std::invalid_argument("Division by zero");
            result = arg1 / arg2;
            break;
        default:
            throw std::invalid_argument("Unsupported operator");
    }
    return result;
}

void DataFrame::toggleWarnings(bool newWarningOutputStatus)
{
    m_enableWarnings = newWarningOutputStatus;
}

double DataFrame::parseDouble(CellAddress &ca)
{
    double dValue = std::numeric_limits<double>::quiet_NaN();
    std::string rawValue = findCell(ca);
    try{
        dValue = std::stod(rawValue);
    }
    catch (const std::invalid_argument& e) {
        if (m_enableWarnings)
            std::cout << "Warning: unable to parse value at " << ca.cName << ca.rName << " into a number!" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Warning: Value at " << ca.cName << ca.rName << " is out of limits for double" << std::endl;
    }
    return dValue;
}

double DataFrame::accessNumericCell(const CellAddress& ca)
{
    return m_NumericData.at(ca.rName).rowData.at(ca.cName);
}
