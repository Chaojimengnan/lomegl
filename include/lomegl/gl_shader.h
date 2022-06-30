#pragma once
#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "lomegl/gl_base.h"

namespace lomegl {

// Throw when error about shader
struct shader_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// This class manage shader program.
class gl_shader : public string_id
{
public:
    gl_shader();
    ~gl_shader();
    gl_shader(const gl_shader&) = delete;
    gl_shader& operator=(const gl_shader&) = delete;
    gl_shader(gl_shader&&) = default;
    gl_shader& operator=(gl_shader&&) = default;

    [[nodiscard]] bool is_vaild() const noexcept;
    [[nodiscard]] unsigned int shader() const noexcept;
    [[nodiscard]] int get_uniform_loc(const char* uniform_name) const noexcept;
    gl_shader& add_vertex(const char* vertex_source);
    gl_shader& add_geometry(const char* geometry_source);
    gl_shader& add_fragment(const char* fragment_source);
    gl_shader& link_shader();
    gl_shader& use();

    template <typename Func, typename... Args>
    gl_shader& uniform(Func func, const char* uniform_name, Args&&... args)
    {
        assert(is_vaild());
        // Must call use() first
        assert([this] {
            int current_program = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
            return !(current_program == 0 || current_program != shader_program_.get());
        }());

        auto iter = uniform_loc_map.find(uniform_name);
        if (iter != uniform_loc_map.end())
        {
            func(iter->second, std::forward<Args>(args)...);
        } else {
            auto loc = get_uniform_loc(uniform_name);
            if (loc == -1)
                throw shader_error(std::string("Can't find uniform name ") + uniform_name);
            uniform_loc_map.emplace(uniform_name, loc);
            func(loc, std::forward<Args>(args)...);
        }
        return *this;
    }

    static constexpr const char* model_name = "model";
    static constexpr const char* view_name = "view";
    static constexpr const char* projection_name = "projection";

private:
    template <typename T>
    void add_source_(T& shader_index, const char* shader_name, const char* source) // NOLINT(bugprone-easily-swappable-parameters)
    {
        static_assert(std::disjunction_v<std::is_same<T, unique_vertex_shader>,
                          std::is_same<T, unique_fragment_shader>,
                          std::is_same<T, unique_geometry_shader>>,
            "T must be one of vaild type");
        const char* temp_str = source;
        lomeglcall(glShaderSource, shader_index.get(), 1, &temp_str, nullptr);
        lomeglcall(glCompileShader, shader_index.get());

        int success = 0;

        lomeglcall(glGetShaderiv, shader_index.get(), GL_COMPILE_STATUS, &success);

        if (success == 0)
        {
            char info_log[512];
            lomeglcall(glGetShaderInfoLog, shader_index.get(), 512, nullptr, info_log);
            lomeglcall(glDeleteShader, shader_index.get());
            shader_index.get() = 0;
            shader_index.release();
            throw shader_error(std::string(shader_name) + " complie fails: " + info_log);
        }

        lomeglcall(glAttachShader, shader_program_.get(), shader_index.get());
    }

    unique_program shader_program_;
    unique_vertex_shader vertex_shader_;
    unique_fragment_shader fragment_shader_;
    unique_geometry_shader geometry_shader_;
    bool is_linked_ = false;
    std::unordered_map<std::string, int> uniform_loc_map;
};

} // namespace lomegl