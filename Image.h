#pragma once
#include <string>
#include <vector>

struct Image
{
    uint8_t label;
    uint32_t rows;
    uint32_t columns;
    std::vector<uint8_t> pixels;
};