#ifndef GPU_MATRIX_HPP
#define GPU_MATRIX_HPP


class GPU_MATRIX {
    public:
    GPU_MATRIX() {

    }
    GPU_MATRIX(const GPU_MATRIX &other) {

    }
    GPU_MATRIX &operator=(const GPU_MATRIX &other) {}

    GPU_MATRIX(GPU_MATRIX &&other) {

    }
    GPU_MATRIX &operator=(GPU_MATRIX &&other) {}

    ~GPU_MATRIX() {}

    bool on_host = true;
    bool on_device = false;

};
#endif
