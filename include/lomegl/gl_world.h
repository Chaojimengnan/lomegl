#pragma once

#include "lomegl/gl_fwd.h"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <lotools/utility.h>

namespace lomegl {
class gl_world
{
public:
    ~gl_world();
    gl_world(const gl_world&) = delete;
    gl_world(gl_world&&) = delete;
    gl_world& operator=(const gl_world&) = delete;
    gl_world& operator=(gl_world&&) = delete;

    // Create a world instance, and set it to current world. **Only one instance is allowd at the same time!!**.
    // Note that the lifetime of `gl_world` should be smaller than the lifetime of OpenGL.
    // If you use GLFW, make sure to destroy the `gl_world` instance before the GLFW is destroyed.
    // This is because `gl_world` need to call the OpengL function to destroy the internal OpenGL object.
    static std::unique_ptr<gl_world> make_world()
    {
        assert(get_current_world_() == nullptr);
        auto new_world = std::unique_ptr<gl_world>(new gl_world);
        get_current_world_() = new_world.get();
        return new_world;
    }

    static gl_world* get_current_world() noexcept
    {
        return get_current_world_();
    }

    gl_world& set_current_camera(const char* obj_name);
    gl_world& set_screen_size(int width, int height) noexcept;
    [[nodiscard]] gl_shader& get_current_shader();
    [[nodiscard]] gl_object& get_current_camera();
    [[nodiscard]] std::string get_current_shader_name() noexcept;
    [[nodiscard]] std::string get_current_camera_name() noexcept;
    [[nodiscard]] std::pair<int, int> get_screen_size() noexcept;
    [[nodiscard]] int get_screen_width() const noexcept;
    [[nodiscard]] int get_screen_height() const noexcept;
    gl_world& set_current_shader_view_mat();
    gl_world& set_current_shader_projection_mat(float fov = 45.0F, float near = 0.1F, float far = 100.0F);
    gl_world& use_shader(const char* shader_name);

    template <typename T, typename... Args>
    constexpr auto& create(const char* obj_name, Args&&... args)
    {
        auto* map = get_map_from_derived_type_<T>();

        assert(map->find(obj_name) == map->cend());
        auto&& item = map->emplace(obj_name, std::make_shared<T>(std::forward<Args>(args)...));
        if (!item.second)
            throw std::runtime_error(std::string("emplace texture ") + obj_name + " fails!");
        return static_cast<T&>(*item.first->second.get());
    }

    // For safety purposes, this function should be used only for a short period of time to use the resource
    // and to ensure that no resources are deleted within the scope of use.
    // To keep a long period reference to a resource, it is highly recommended to set `safety_ref` to true.
    template <typename T, bool safety_ref = false>
    [[nodiscard]] constexpr decltype(auto) get(const char* obj_name)
    {
        auto* map = get_map_from_derived_type_<T>();
        auto&& result = map->find(obj_name);

        if (result == map->cend())
            throw std::runtime_error(std::string("Can't find key ") + obj_name);

        // If T equal to base_type, then the type conversion is not necessary
        using TestType = lot::any_type_true_t<true, std::is_same, T, gl_texture, gl_shader, gl_vertex, gl_object>;
        if constexpr (std::is_same_v<TestType, void>) // derived
        {
            if constexpr (safety_ref)
                return std::weak_ptr { std::dynamic_pointer_cast<T>(result->second) };
            else
                return *(dynamic_cast<T*>(result->second.get()));

        } else {
            if constexpr (safety_ref)
                return std::weak_ptr { result->second };
            else
                return *(result->second.get());
        }
    }

    template <typename T>
    constexpr auto remove(const char* obj_name)
    {
        auto* map = get_map_from_derived_type_<T>();
        return map->erase(obj_name);
    }

    template <typename T>
    [[nodiscard]] constexpr bool exists(const char* obj_name) const
    {
        auto* map = get_map_from_derived_type_<T>();
        return map->contains(obj_name);
    }

private:
    gl_world();

    static gl_world*& get_current_world_() noexcept
    {
        static gl_world* current_world_ = nullptr; // NOLINT
        return current_world_;
    }

    template <typename T>
    constexpr auto* get_map_from_derived_type_() const noexcept
    {
        using BaseType = lot::any_type_true_t<false, std::is_base_of, T, gl_texture, gl_shader, gl_vertex, gl_object>;
        static_assert(!std::is_same_v<BaseType, void>, "T must be or derived from gl_texture, gl_shader, gl_vertex, or gl_object");

        const std::unordered_map<std::string, std::shared_ptr<BaseType>>* map = nullptr;
        if constexpr (std::is_base_of_v<gl_texture, T>)
        {
            map = &texture_;
        } else if constexpr (std::is_base_of_v<gl_shader, T>)
        {
            map = &shader_;
        } else if constexpr (std::is_base_of_v<gl_vertex, T>)
        {
            map = &vertex_;
        } else if constexpr (std::is_base_of_v<gl_object, T>)
        {
            map = &object_;
        }
        return map;
    }

    template <typename T>
    constexpr auto* get_map_from_derived_type_() noexcept
    {
        using BaseType = lot::any_type_true_t<false, std::is_base_of, T, gl_texture, gl_shader, gl_vertex, gl_object>;
        static_assert(!std::is_same_v<BaseType, void>, "T must be or derived from gl_texture, gl_shader, gl_vertex, or gl_object");
        return const_cast<std::unordered_map<std::string, std::shared_ptr<BaseType>>*>(static_cast<const gl_world*>(this)->get_map_from_derived_type_<T>()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    std::unordered_map<std::string, std::shared_ptr<gl_texture>> texture_;
    std::unordered_map<std::string, std::shared_ptr<gl_shader>> shader_;
    std::unordered_map<std::string, std::shared_ptr<gl_vertex>> vertex_;
    std::unordered_map<std::string, std::shared_ptr<gl_object>> object_;
    std::string current_camera_;
    std::string current_shader_;
    std::pair<int, int> screen_size_ = { 0, 0 };
};

} // namespace lomegl
