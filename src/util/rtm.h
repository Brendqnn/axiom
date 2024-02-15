#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif __linux__
#include <sys/sysinfo.h>
#else
#error "Unsupported platform"
#endif

double get_memory_usage_mb() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        SIZE_T phys_mem_used_by_process = pmc.PrivateUsage;
        return (double)phys_mem_used_by_process / (1024 * 1024);
    }
#elif __linux__
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long totalPhysMem = memInfo.totalram;
    totalPhysMem += memInfo.totalswap;
    totalPhysMem *= memInfo.mem_unit;
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    physMemUsed += memInfo.totalswap - memInfo.freeswap;
    physMemUsed *= memInfo.mem_unit;
    return (double)physMemUsed / (1024 * 1024);
#endif
    return 0.0; // Failed to get memory usage
}
