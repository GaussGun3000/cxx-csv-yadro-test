#include <iostream>

#include "DataFrame.h"

int main()
{
    DataFrame df("../test.csv");
    df.translateToNumeric();
    return 0;
}

