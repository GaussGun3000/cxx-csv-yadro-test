#include <iostream>

#include "DataFrame.h"
#include <memory>

int main()
{
    try
    {
        std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>("test.csv");
        df->toggleWarnings(true);
        df->translateToNumeric();
        df->printData();
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong:\n" << e.what();
    }
    return 0;
}

