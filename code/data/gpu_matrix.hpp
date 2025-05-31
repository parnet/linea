#pragma once


class GPU_MATRIX {
    public:
    GPU_MATRIX() {

    }
    GPU_MATRIX(const GPU_MATRIX &other) {

    }
    GPU_MATRIX &operator=(const GPU_MATRIX &other) {
        if (this != &other) {

        }
        return *this;
    }

    GPU_MATRIX(GPU_MATRIX &&other) noexcept {

    }
    GPU_MATRIX &operator=(GPU_MATRIX &&other) noexcept {
        if (this != &other) {

        }
        return *this;
    }

    ~GPU_MATRIX() {}

    // -----------------------------------------------------------------------------------------------------------------

    bool on_host = true;
    bool on_device = false;

};
