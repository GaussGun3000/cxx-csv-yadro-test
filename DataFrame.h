#ifndef YADRO_CSV_TEST_DATAFRAME_H
#define YADRO_CSV_TEST_DATAFRAME_H

#include <vector>
#include <string>
#include <type_traits>

template<typename T>
struct DataTypeAllowed {
    static constexpr bool value = std::is_same<T, int>::value
                                  || std::is_same<T, double>::value
                                  || std::is_same<T, std::string>::value;
};

template <typename T>
struct Row
{
    static_assert(DataTypeAllowed<T>::value, "Unsupported data type");
    std::vector<T> rowData;
};

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

private:
    std::vector<std::string> m_columnNames;
    std::vector<std::string> m_rowNames;
    std::vector<Row<std::string>> m_rawData;
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiters);

};


#endif //YADRO_CSV_TEST_DATAFRAME_H
