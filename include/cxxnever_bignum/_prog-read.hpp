#pragma once
#include <stdexcept>
#include <string_view>
#include <vector>
#include <charconv>
#include <cstdint>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_read
{
    void read(std::vector<type_t>& num, std::string_view str)
    {
        num.reserve((str.size() + 1) / 3 / sizeof(type_t));

        int index = 0;
        type_t value = 0;
        while (str.size()) {
            if (str.size() < 2)
                throw std::runtime_error("invalid format");
            char input[3] = {str[0], str[1], 0};
            uint8_t byte = 0;
            auto ret = std::from_chars(&input[0], &input[2], byte, 16);
            if (ret.ec != std::errc() || ret.ptr != &input[2])
                throw std::runtime_error("invalid format");
            value <<= 8;
            value |= byte;
            index++;
            if (index == sizeof(type_t))
                num.push_back(value), value = 0, index = 0;
            if (str.size() == 2)
                break;
            if (str[2] != ':')
                throw std::runtime_error("invalid format");
            str.remove_prefix(3);
        }

        if (index)
            throw std::runtime_error("invalid format");

        if (num.size())
            for (size_t a = 0, b = num.size() - 1; a < b; a++, b--)
                std::swap(num[a], num[b]);
    }
};

}
