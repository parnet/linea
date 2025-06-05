//
// Created by maro on 2025-06-03.
//

#ifndef MI300A_HPP
#define MI300A_HPP


struct Core {

};

struct Processor {
    Core* cores = new Core[8];

};

struct ComputeCore  {
    StreamProcessors * stream_processors = new StreamProcessors[64];
};
struct GPU {
    ComputeCore * compute_core = new ComputeCore[38];
    MatrixCores * matrix_cores = new MatrixCores[152];
};

struct MemoryStack {
    int size = 16; // GB!
};


struct Package {
    Processor * processor = new Processor[3];
    GPU * gpu = new GPU[6];
    MemoryStack * memory_stack = new MemoryStack[8];
};



#endif //MI300A_HPP
