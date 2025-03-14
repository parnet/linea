#ifndef UTIL_VERSION_HPP
#define UTIL_VERSION_HPP

#include <cstdint>
#include <sstream>
#include <string>

class Version{
public:
    Version() = default;

    Version(uint8_t majnum, uint8_t minnum, uint8_t relnum) : _majnum(majnum), _minnum(_minnum), _relnum(relnum) {

    }
    ~Version() = default;

    std::string repres() const {
        std::stringstream ss;
        ss << std::to_string(_majnum) << "."
            << std::to_string(_minnum) << "."
            << std::to_string(_relnum)
            << std::endl;
        return ss.str();
    }

    std::string str() {
        return this->repres();
    }
private:
    uint8_t _majnum = 0;
    uint8_t _minnum = 0;
    uint8_t _relnum = 0;
};

#endif
