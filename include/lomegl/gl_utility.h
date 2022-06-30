#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace lomegl {

using image_data_ptr = std::unique_ptr<unsigned char, void (*)(void*)>;

struct image_info
{
    int width;
    int height;
    int channel;
};

struct image_data
{
    image_data_ptr data;
    image_info info;
};

std::string get_content_from_file(const char* path);

image_data get_image_from_file(const char* path, bool flip_vertically = true);
image_data get_image_from_memory(const unsigned char* buffer, int size, bool flip_vertically = true);
image_info get_image_info_from_memory(const unsigned char* buffer, int size, bool flip_vertically = true);

} // namespace lomegl