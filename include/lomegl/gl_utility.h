#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace lomegl {

using image_data_ptr = std::unique_ptr<unsigned char, void (*)(void*)>;

struct image_data
{
    image_data_ptr data;
    int width;
    int height;
    int channel;
};

std::string get_content_from_file(std::string_view path);

image_data get_image_from_file(std::string_view path, bool flip_vertically = true);

} // namespace lomegl