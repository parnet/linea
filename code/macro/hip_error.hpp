#define HIP_CHECK(cmd) {                                                           \
    hipError_t error = cmd;                                                        \
    if(error != hipSuccess) [[unlikely]]                                           \
    {                                                                               \
    std::cerr << "error: " << hipGetErrorString(error) << " (" << error << ") at " \
    << __FILE__ << ":" << __LINE__ << "\n";                               \
    exit(EXIT_FAILURE);                                                             \
    }                                                                               \
}