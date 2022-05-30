#pragma once

#include <string>

namespace lomegl {
constexpr auto MYGL_SOURCE_DIR = "D:/base/current_code/our_projects/lomegl/test";

inline std::string from_source_dir(const char* target)
{
    return std::string(MYGL_SOURCE_DIR) + target;
    // return std::string(".") + v;
}
} // namespace lomegl
