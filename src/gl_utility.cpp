#include "lomegl/gl_utility.h"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "lomegl/thirdparty/stb_image.h"

namespace lomegl {
std::string get_content_from_file(std::string_view path)
{
    std::ifstream in_stream(path.data());
    if (!in_stream)
        throw std::runtime_error(std::string("Open file ") + path.data() + " fails");
    std::stringstream out;
    out << in_stream.rdbuf();
    return out.str();
}

image_data get_image_from_file(std::string_view path, bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(static_cast<int>(flip_vertically));
    int width {};
    int height {};
    int nrChannels {};
    unsigned char* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
    if (data == nullptr)
        throw std::runtime_error(std::string("Load image from ") + path.data() + " fails");
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data_ptr(data, stbi_image_free);

    return { std::move(data_ptr), width, height, nrChannels };
}

} // namespace lomegl