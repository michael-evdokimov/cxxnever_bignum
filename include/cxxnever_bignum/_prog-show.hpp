#pragma once
#include <cstdint>
#include <vector>
#include <string>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_show
{
    std::string show(const std::vector<type_t>& num)
    {
        std::string r = {};

        size_t bytes = num.size() * sizeof(type_t);
        r.reserve(bytes * 2 + bytes - !!bytes);

        const char* prefix = "";
        char buf[4] = {0};
        for (size_t i = num.size(); i--; ) {
            type_t value = num[i];
            for (int8_t j = sizeof(type_t); j--; ) {
                int8_t x = value >> j * 8;
                snprintf(buf, sizeof(buf), "%s%02hhx", prefix, x);
                r += buf;
                prefix = ":";
            }
        }

        return r;
    }
};

}
