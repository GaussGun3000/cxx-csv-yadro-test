#include <iostream>

#include "DataFrame.h"
#include <limits>
#include <memory>
#include <cmath>

int main()
{
    std::string testTitle;
    try
    {
        testTitle = "Invalid cell reference in formula";
        std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test_invalid_cell_reference.csv");
        df->translateToNumeric();
        double invalidCellValue = df->accessNumericCell(CellAddress("BC", "30"));
        if (std::isnan(invalidCellValue))
            std::cout << testTitle <<" test - passed\n";
        else
            std::cout << testTitle << " test - failed\n";
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong in  " << testTitle <<std::endl << e.what() << std::endl;
    }
    try
    {
        testTitle = "Invalid characters";
        std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test_invalid_chars.csv");
        df->translateToNumeric();
        double invalidCellValue7 = df->accessNumericCell(CellAddress("FAB", "7"));
        double invalidCellValue9 = df->accessNumericCell(CellAddress("FAB", "9"));
        double invalidCellValue1 = df->accessNumericCell(CellAddress("FAB", "1"));
        if (std::isnan(invalidCellValue1) &&  std::isnan(invalidCellValue7) && std::isnan(invalidCellValue9))
            std::cout << testTitle <<" test - passed\n";
        else
            std::cout << testTitle << " test - failed\n";
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong in  " << testTitle <<std::endl << e.what() << std::endl;
    }
    try
    {
        testTitle = "Double value overflow";
        std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test_overflow.csv");
        df->translateToNumeric();
        double inf = std::numeric_limits<double>::infinity();
        double invalidCellValueC4 = df->accessNumericCell(CellAddress("C", "4"));
        double invalidCellValueD4 = df->accessNumericCell(CellAddress("D", "4"));
        if (invalidCellValueC4 == inf && invalidCellValueD4 == inf)
            std::cout << testTitle <<" test - passed\n";
        else
            std::cout << testTitle << " test - failed\n";
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong in  " << testTitle <<std::endl << e.what() << std::endl;
    }
    try
    {
        testTitle = "Not enough columns in a row";
        const std::string expectedExceptionText = "Not enough columns in row";
        try
        {
            std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test_less_columns.csv");
            df->translateToNumeric();
        }catch (std::out_of_range& e)
        {
            std::string exceptionText = e.what();
            if (exceptionText.find(exceptionText) != std::string::npos)
                std::cout << testTitle << " test - passed\n";
            else
                std::cout << testTitle << " test - failed\n";
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong in  " << testTitle <<std::endl << e.what() << std::endl;
    }
    try
    {
        testTitle = "Duplicate row names";
        const std::string expectedExceptionText = "Unable to parse CSV file, duplicate row names are present!";
        try
        {
            std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test_duplicate_rows.csv");
            df->translateToNumeric();
        }catch (std::runtime_error& e)
        {
            std::string exceptionText = e.what();
            if (exceptionText.find(exceptionText) != std::string::npos)
                std::cout << testTitle << " test - passed\n";
            else
                std::cout << testTitle << " test - failed\n";
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong in  " << testTitle <<std::endl << e.what() << std::endl;
    }
    getchar();
    return 0;
}

