#include <iostream>

#include "DataFrame.h"
#include <memory>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "No filename argument was provided!\n";
        return -1;
    }

    std::string filename = argv[1];
    bool warnings = false;
    if (argc >= 3)
    {
        std::string arg2 = argv[2];
        warnings = (arg2  == "true");
    }
    try
    {
        std::unique_ptr<DataFrame> df = std::make_unique<DataFrame>(filename);
        df->toggleWarnings(warnings);
        df->translateToNumeric();
        df->printData();
    }
    catch (std::exception& e)
    {
        std::cout << "Something went wrong:\n" << e.what();
    }
    return 0;
}

