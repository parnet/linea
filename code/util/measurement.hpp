#pragma once

#include <map>
#include <sstream>
#include <string>
#include <vector>

class Measurement{
public:
    Measurement() = default;
    ~Measurement() = default;

    void add(const std::string &key, double value) {
        this->data[key].push_back(value);
    }

    std::string csv() {
        std::stringstream ss;
        return ss.str();
    }
private:
    std::vector<std::string> keys;
    std::map<std::string,std::vector<double>> data;
};
