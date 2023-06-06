#ifndef YADRO_CSV_TEST_DATAFRAME_H
#define YADRO_CSV_TEST_DATAFRAME_H

#include <vector>
#include <set>
#include "DFSupportStructs.h"

class DataFrame
{
public:
    DataFrame(const std::string& filePath, char delimiter = ',');
    DataFrame(const std::string& filePath, uint32_t rcNamingMode, char delimiter = ',');

    enum class rcNamingModes {rNUMBERS_cLETTERS = 0};
    void printData(uint32_t rows = 0, uint32_t columns = 0);
    void translateToNumeric();
    std::vector<Row<std::string>> getRawData;
    std::vector<Row<double>> getNumericData;
    void toggleWarnings(bool newWarningOutputStatus);
    double accessNumericCell(const CellAddress& ca);

private:
    std::set<std::string, StringWeightComparator> m_columnNames;
    std::set<std::string, StringWeightComparator> m_rowNames;
    std::vector<std::string> m_columnNamesOriginalOrder;
    std::map<std::string, Row<std::string>> m_rawData;
    std::map<std::string, Row<double>> m_NumericData;
    bool m_enableWarnings = false;

    double parseDouble(CellAddress& ca);
    std::string findCell(CellAddress& ca);
    void readColNames(std::ifstream& file, char delimiter = ',');
    void readData(std::ifstream& file, char delimiter = ',');
    static double arithmeticOperation(double arg1, double arg2, char op);
    CellAddress parseCellAddress(const std::string& str);
    double parseFormula(const std::string &formula, CellAddress &currentCell, CellAddress &originalCaller,
                        std::set<CellAddress> &callingCells);
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiters = ",");
};


#endif //YADRO_CSV_TEST_DATAFRAME_H
