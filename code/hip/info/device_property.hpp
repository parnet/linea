#pragma once
#ifdef USE_HIP
#include <hip/hip_runtime.h>



void hardware_limit(){
hipDeviceProp_t prop;
hipGetDeviceProperties(&prop, 0);

printf("Max threads per block: %d\n", prop.maxThreadsPerBlock);
printf("Max block dim: x=%d y=%d z=%d\n",
       prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);

printf("Max grid dim: x=%d y=%d z=%d\n",
       prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
}
#endif