#ifndef GENERIC_COLOR_H
#define GENERIC_COLOR_H

#include <array>
#include <cstdint>
#include <ostream>

enum class color_e : uint8_t
{
    RED = 0,
    GREEN = 1,
    BLUE = 2
};

static std::array<std::string, 3> color_to_string{
    "RED",
    "GREEN",
    "BLUE",
};

class Color
{
public:
    Color(color_e _c) : c(_c) {}

    friend auto operator<=>(const Color &l, const Color &r)
    {
        return static_cast<uint8_t>(l.c) <=> static_cast<uint8_t>(r.c);
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &obj)
    {
        os << color_to_string[static_cast<uint8_t>(obj.c)];
        return os;
    }

private:
    color_e c;
};

#endif // GENERIC_COLOR_H