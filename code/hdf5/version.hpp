#pragma once
#include <H5public.h>
#include "../util/version.hpp"

Version version_hdf5() {
    unsigned majnum, minnum, relnum;
    H5get_libversion(&majnum, &minnum, &relnum);
    return Version(majnum,minnum,relnum);

}
