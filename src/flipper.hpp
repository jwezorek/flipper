#pragma once

#include <vector>
#include "hex_grid.hpp"

namespace jw {

    struct flipper_level {
        hex_grid initial_configuration;
        hex_grid target;
        std::vector<hex_grid> stamps;
    };


}