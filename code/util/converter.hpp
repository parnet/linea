#ifndef UTIL_CONVERTER_HPP
#define UTIL_CONVERTER_HPP

enum Unit {
    NONE,
    Kilo,
    Mega,
    Giga,
    Terra,
    Peta,
    Exa
};

inline double convert(size_t value, Unit unit ) {
    switch (unit) {
        case NONE:
            return value;
        case Kilo:
            return value/1024.0;
        case Mega:
            return value/1048576.0;
        case Giga:
            return value/1073741824.0;
        case Terra:
            return value/1099511627776.0;
        case Peta:
            return value/1125899906842624.0;
        case Exa:
            return value/1152921504606846976.0;
        default:
            return value;
    }
}

inline std::string symbol(Unit unit ) {
    switch (unit) {
        case NONE:
            return "B";
        case Kilo:
            return "KiB";
        case Mega:
            return "MiB";
        case Giga:
            return "GiB";
        case Terra:
            return "TiB";
        case Peta:
            return "PiB";
        case Exa:
            return "EiB";
        default:
            return "Undefined";
    }
}

#endif