
int main(int argc, char *argv[]) {
    std::cout << "Accelerator:" << std::endl;
#ifdef USE_HIP
    std::cout << "    using HIP" << ;
    std::string gfx_version = "11.0.0";
    if (argc > 1) {
        gfx_version = argv[1];
    }
    std::string env_var = "HSA_OVERRIDE_GFX_VERSION=" + gfx_version;
    putenv(const_cast<char*>(env_var.c_str()));
    std::cout << " with GFX version " << gfx_version << std::endl;
#endif

    StdVector vector = StdVector(21);
    const auto dsize = static_cast<double>(21) -1;
    auto fun = [dsize](size_t i){return sin(static_cast<double>(i)* M_PI * (static_cast<double>(i) / (dsize)));};
    create_fun(vector,fun);

}
