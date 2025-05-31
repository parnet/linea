#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

template <typename T, std::size_t Alignment>
struct AlignedAllocator {
    using value_type = T;

    T* allocate(std::size_t n) {
        void* ptr = std::aligned_alloc(Alignment, n * sizeof(T));
        if (!ptr) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }

    void deallocate(T* p, std::size_t) noexcept {
        std::free(p);
    }
};

//   std::vector<float, AlignedAllocator<float, ALIGNMENT>> aligned_vec(ELEMENTS, 0.0f);