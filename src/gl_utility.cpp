#include "lomegl/gl_utility.h"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "lomegl/thirdparty/stb_image.h"

namespace lomegl {
std::string get_content_from_file(const char* path)
{
    std::ifstream in_stream((path));
    if (!in_stream) [[unlikely]]
        throw std::runtime_error(std::string("Open file ") + path + " fails");
    std::stringstream out;
    out << in_stream.rdbuf();
    return out.str();
}

image_data get_image_from_file(const char* path, bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(static_cast<int>(flip_vertically));
    int width {};
    int height {};
    int nrChannels {};
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == nullptr) [[unlikely]]
        throw std::runtime_error(stbi_failure_reason());
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data_ptr(data, stbi_image_free);

    return { std::move(data_ptr), { width, height, nrChannels } };
}

image_info get_image_info_from_memory(const unsigned char* buffer, int size, bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(static_cast<int>(flip_vertically));
    int width {};
    int height {};
    int nrChannels {};

    if (stbi_info_from_memory(buffer, size, &width, &height, &nrChannels) != 1)
    {
        throw std::runtime_error(stbi_failure_reason());
    }

    return { width, height, nrChannels };
}

image_data get_image_from_memory(const unsigned char* buffer, int size, bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(static_cast<int>(flip_vertically));
    int width {};
    int height {};
    int nrChannels {};

    unsigned char* data = stbi_load_from_memory(buffer, size, &width, &height, &nrChannels, 0);
    if (data == nullptr) [[unlikely]]
        throw std::runtime_error(stbi_failure_reason());
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data_ptr(data, stbi_image_free);

    return { std::move(data_ptr), { width, height, nrChannels } };
}

} // namespace lomegl