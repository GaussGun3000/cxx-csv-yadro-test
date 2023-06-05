#ifndef YADRO_CSV_TEST_DFSUPPORTSTRUCTS_H
#define YADRO_CSV_TEST_DFSUPPORTSTRUCTS_H

#endif //YADRO_CSV_TEST_DFSUPPORTSTRUCTS_H

#include <type_traits>
#include <map>
#include <string>

inline uint32_t getStringWeight(const std::string& str)
{
    uint32_t sum = 0;
    for(const char& c: str)
        sum += c;
    return sum;
}

struct StringWeightComparator
{
    bool operator()(const std::string &str1, const std::string &str2)
    {
        uint32_t w1 = getStringWeight(str1);
        uint32_t w2 = getStringWeight(str2);
        return w1 < w2;
    }
};

template<typename T>
struct DataTypeAllowed {
    static constexpr bool value = std::is_same<T, int>::value
                                  || std::is_same<T, double>::value
                                  || std::is_same<T, std::string>::value;
};

template <typename T>

struct Row
{
    Row() = default;
    static_assert(DataTypeAllowed<T>::value, "Unsupported data type");
    std::map<std::string, T> rowData;
};

struct CellAddress
{
    CellAddress(): cName(), rName() {};
    CellAddress(const std::string& col, const std::string& row): cName(col), rName(row) {}
    std::string cName;
    std::string rName;

    bool operator< (const CellAddress& rhs) const
    {
        if (this->cName == rhs.cName)
            return (std::stoi(this->rName) < std::stoi(rhs.rName));
        else
            return (getStringWeight(this->cName) < getStringWeight(rhs.cName));
    }
    bool operator== (const CellAddress& rhs) const
    { return (this->cName == rhs.cName) && (this->rName == rhs.rName); }

};