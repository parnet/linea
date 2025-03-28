
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>


#include "sys/types.h"
#include "sys/sysinfo.h"
#include "memory.hpp"

namespace memory{

    unsigned long get_physical_memory_total(){
        struct sysinfo memInfo;
        unsigned long totalPhysMem = memInfo.totalram;
        totalPhysMem *= memInfo.mem_unit;
        return totalPhysMem;
    }

    unsigned long get_physical_memory_free(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.freeram;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }

    unsigned long get_physical_memory_shared(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.sharedram;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }

    unsigned long get_physical_memory_buffer(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.bufferram;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }

    unsigned long get_physical_swap_total(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.totalswap;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }



    unsigned long get_physical_memory_swap_free(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.freeswap;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }

    unsigned long get_memory_high(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.totalhigh;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }
    unsigned long get_memory_high_free(){
        struct sysinfo memInfo;
        unsigned long physMemFree = memInfo.freehigh;
        physMemFree *= memInfo.mem_unit;
        return physMemFree;
    }




    unsigned long get_physical_memory_used(){
        struct sysinfo memInfo;
        unsigned long physMemUsed = memInfo.totalram - memInfo.freeram;
        physMemUsed *= memInfo.mem_unit;
        return physMemUsed;
    }


    unsigned long get_virtual_memory_total(){
        struct sysinfo memInfo;
        sysinfo (&memInfo);
        unsigned long totalVirtualMem = memInfo.totalram;
        totalVirtualMem += memInfo.totalswap;

        totalVirtualMem *= memInfo.mem_unit;
        return totalVirtualMem;
    }

    unsigned long get_virtual_memory_used(){
        struct sysinfo memInfo;
        sysinfo (&memInfo);
        unsigned long virtualMemUsed = memInfo.totalram - memInfo.freeram;
        virtualMemUsed += memInfo.totalswap - memInfo.freeswap;

        virtualMemUsed *= memInfo.mem_unit;
        return virtualMemUsed;
    }

    unsigned long get_virtual_memory_consumed(){
        FILE *file = fopen("/proc/self/status", "r");
        unsigned long result = -1;
        char line[128];

        while (fgets(line, 128, file) != nullptr) {
            if (strncmp(line, "VmSize:", 7) == 0) {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    unsigned long get_virtual_memory_peak(){
        FILE *file = fopen("/proc/self/status", "r");
        unsigned long result = -1;
        char line[128];

        while (fgets(line, 128, file) != nullptr) {
            if (strncmp(line, "VmPeak:", 7) == 0) {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    unsigned long get_physical_memory_consumed() {
        FILE *file = fopen("/proc/self/status", "r");
        unsigned long result = -1;
        char line[128];

        while (fgets(line, 128, file) != nullptr) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }

    unsigned long get_physical_memory_peak(){
        FILE *file = fopen("/proc/self/status", "r");
        unsigned long result = -1;
        char line[128];

        while (fgets(line, 128, file) != nullptr) {
            if (strncmp(line, "VmHWM:", 6) == 0) {
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;

    }


    unsigned long parseLine(char *line){
        unsigned long i = strlen(line);
        const char *p = line;
        while (*p < '0' || *p > '9') p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
    }

    void info(Unit unit) {
        std::string m_symbol = symbol(unit);
        auto virtual_memory_total = convert(get_virtual_memory_total(),unit);
        std::cout << "virtual_memory_total:     " << virtual_memory_total << m_symbol << std::endl;
        auto virtual_memory_used = convert(get_virtual_memory_used(),unit);
        std::cout << "virtual_memory_used:      " << virtual_memory_used << m_symbol << std::endl;
        auto virtual_memory_consumed = convert(get_virtual_memory_consumed(),unit);
        std::cout << "virtual_memory_consumed:  " << virtual_memory_consumed << m_symbol << std::endl;
        auto virtual_memory_peak = convert(get_virtual_memory_peak(),unit);
        std::cout << "virtual_memory_peak:      " << virtual_memory_peak << m_symbol << std::endl;
        auto physical_memory_total = convert(get_physical_memory_total(),unit);
        std::cout << "physical_memory_total:    " << physical_memory_total << m_symbol << std::endl;
        auto physical_memory_used = convert(get_physical_memory_used(),unit);
        std::cout << "physical_memory_used:     " << physical_memory_used << m_symbol << std::endl;
        auto physical_memory_free = convert(get_physical_memory_free(),unit);
        std::cout << "physical_memory_free:     " << physical_memory_free << m_symbol << std::endl;
        auto physical_memory_consumed = convert(get_physical_memory_consumed(),unit);
        std::cout << "physical_memory_consumed: " << physical_memory_consumed << m_symbol << std::endl;
        auto physical_memory_peak =convert( get_physical_memory_peak(),unit);
        std::cout << "physical_memory_peak:     " << physical_memory_peak << m_symbol << std::endl;
    }
}
