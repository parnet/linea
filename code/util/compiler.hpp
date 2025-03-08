//
// Created by maro on 2025-03-06.
//

#ifndef COMPILER_HPP
#define COMPILER_HPP

void print_arch() {

    std::cout
#ifdef __SSE__
<< "SSE "
#endif
#ifdef __MMX__
<< "MMX "
#endif
#ifdef __SSE2__
    << "SSE2 "
#endif
#ifdef __SSE3__
    << "SSE3 "
#endif
#ifdef __SSE4A___
    << "SSE4A "
#endif
#ifdef __SSE_MATH__
    << "SSE_MATH "
#endif

#ifdef __SSE4_1__
    << "SSE4_1 "
#endif
#ifdef __SSE4_2__
    << "SSE4_2 "
#endif


#ifdef __AVX__
<< "AVX "
#endif

#ifdef __AVX2__
<< "AVX2 "
#endif

#ifdef __AVX512F__
<< "AVX512F "
#endif
#ifdef __AVX512BF16__
<< "AVX512BF16 "
#endif
#ifdef __AVX512BW__
    << "AVX512BW "
#endif
#ifdef __BMI__
    << "BMI "
#endif
#ifdef __BMI2__
    << "BMI2 "
#endif
#ifdef __RDSEED__
<< "RDSEED "
#endif
#ifdef __RDRND__
<< "RDRND "
#endif
#ifdef __F16C__
<< "F16C "
#endif

    << std::endl << std::flush;
}
#endif //COMPILER_HPP
