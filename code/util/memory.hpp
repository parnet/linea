#ifndef MEMORY_HPP
#define MEMORY_HPP


#include "converter.hpp"

namespace memory {
    void info(Unit unit = NONE);

    unsigned long get_physical_memory_total();
    unsigned long get_physical_memory_free();
    unsigned long get_physical_memory_shared();
    unsigned long get_physical_memory_buffer();
    unsigned long get_physical_swap_total();
    unsigned long get_physical_memory_swap_free();
    unsigned long get_memory_high();
    unsigned long get_memory_high_free();
    unsigned long get_physical_memory_used();
    unsigned long get_virtual_memory_total();
    unsigned long get_virtual_memory_used();
    unsigned long get_virtual_memory_consumed();
    unsigned long get_virtual_memory_peak();
    unsigned long get_physical_memory_consumed();
    unsigned long get_physical_memory_peak();
    unsigned long parseLine(char* line);


};


#endif
