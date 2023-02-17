#ifndef GENERIC_COLOR_H
#define GENERIC_COLOR_H

#include <cstdint>
#include <ostream>

class Color
{
public:
    enum class color_e : uint8_t
    {
        RED = 0,
        GREEN = 1,
        BLUE = 2
    };

    Color(color_e _c) : c(_c) {}

    friend bool operator<(const Color &l, const Color &r)
    {
        return static_cast<uint8_t>(l.c) < static_cast<uint8_t>(r.c);
    }

    friend bool operator==(const Color &l, const Color &r)
    {
        return static_cast<uint8_t>(l.c) == static_cast<uint8_t>(r.c);
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &obj)
    {
        switch (obj.c)
        {
        case color_e::RED:
        {
            os << "RED";
            break;
        }
        case color_e::GREEN:
        {
            os << "GREEN";
            break;
        }
        case color_e::BLUE:
        {
            os << "BLUE";
            break;
        }
        }

        return os;
    }

private:
    color_e c;
};

#endif // GENERIC_COLOR_H