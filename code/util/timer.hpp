#pragma once

#include <chrono>

class Timer {
public:
    using time_value = std::chrono::duration<double>;
    using time_point = std::chrono::high_resolution_clock::time_point;
    using time_provider = std::chrono::high_resolution_clock;
    //--------------------------------------------------------------------------------------------------------------

    time_point t0;
    time_point t1;
    time_point t2;

    //--------------------------------------------------------------------------------------------------------------

    Timer() {
        t0 = time_provider::now();
    }

    ~Timer() = default;

    //--------------------------------------------------------------------------------------------------------------

    void start() {
        t0 = time_provider::now();
    }

    void stop() {
        t1 = time_provider::now();
    }

    void now(double& total) {
        t2 = time_provider::now();
        const std::chrono::duration<double> t0diff = std::chrono::duration_cast<time_value>(t2 - t0);
        total = t0diff.count();
    }

    double get() const {
        const std::chrono::duration<double> difference = std::chrono::duration_cast<time_value >(t1 - t0);
        return difference.count();
    }

};

