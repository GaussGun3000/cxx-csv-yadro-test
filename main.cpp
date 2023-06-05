#include <iostream>

#include "DataFrame.h"

int main()
{
    DataFrame df("test3.csv");
    df.translateToNumeric();
    df.printData();
    return 0;
}

