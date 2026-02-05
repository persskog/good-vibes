#include "pch.h"
#include "parsing.hpp"

int main()
{
    try
    {
        auto on_exit = Initialize();
        RunAsync().get();
    }
    catch (...) {}
    return 0;
}

