#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <lomegl/gl_base.h>
#include <lomegl/gl_exception.h>
#include <lomegl/gl_object.h>
#include <lomegl/gl_shader.h>
#include <lomegl/gl_texture.h>
#include <lomegl/gl_utility.h>
#include <lomegl/gl_vertex.h>
#include <lomegl/gl_world.h>
#include <lomegl/loglfw/glfw_utility.h>

#include "cmake_path.h"

// 自定义函数，处理滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// 创建并读取顶点，着色器，纹理等资源，并创建实体
void init_resource();

struct my_vertex
{
    struct pos_
    {
        float x;
        float y;
        float z;
    } pos;
    struct texture_pos_
    {
        float x;
        float y;
    } texture_pos;
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cert-err58-cpp)
glm::vec3 cubePositions[] = {
    glm::vec3(0.0F, 0.0F, 0.0F),
    glm::vec3(2.0F, 5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F),
    glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3(2.4F, -0.4F, -3.5F),
    glm::vec3(-1.7F, 3.0F, -7.5F),
    glm::vec3(1.3F, -2.0F, -2.5F),
    glm::vec3(1.5F, 2.0F, -2.5F),
    glm::vec3(1.5F, 0.2F, -1.5F),
    glm::vec3(-1.3F, 1.0F, -1.5F)
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
my_vertex vertices[] = {
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 0.0F } },
    { { 0.5F, -0.5F, -0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, 0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { 0.5F, 0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { -0.5F, 0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 0.0F } },
    { { -0.5F, -0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { 0.5F, -0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 1.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 1.0F } },
    { { -0.5F, 0.5F, 0.5F }, { 0.0F, 1.0F } },
    { { -0.5F, -0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { -0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { -0.5F, 0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { -0.5F, -0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { -0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, 0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { 0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { 0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { 0.5F, -0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { 0.5F, -0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { 0.5F, -0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, -0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { -0.5F, -0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { -0.5F, -0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { -0.5F, 0.5F, -0.5F }, { 0.0F, 1.0F } },
    { { 0.5F, 0.5F, -0.5F }, { 1.0F, 1.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { 0.5F, 0.5F, 0.5F }, { 1.0F, 0.0F } },
    { { -0.5F, 0.5F, 0.5F }, { 0.0F, 0.0F } },
    { { -0.5F, 0.5F, -0.5F }, { 0.0F, 1.0F } }
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
float fov = 45.0F;

int main()
{
    // 使用RAII的类的实例，会显示初始化GLFW，并在退出main前调用GLFW销毁函数
    lomegl::glfw_control mygl_control;

    try
    {
        // 创建窗口
        GLFWwindow* window = lomegl::glfw_utility::init();

        // 设置glfw回调
        glfwSetFramebufferSizeCallback(window, lomegl::glfw_utility::on_window_size_change);
        glfwSetCursorPosCallback(window, lomegl::glfw_utility::mouse_callback);
#if !defined(__linux__)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
        glfwSetScrollCallback(window, scroll_callback);

        // 初始化资源并创建实体
        init_resource();

        // 世界设置
        auto&& world_instance = lomegl::gl_world::instance();
        // 设置屏幕大小为800, 600
        world_instance.set_screen_size(800, 600);
        // 将当前相机设置为"my_camera"
        world_instance.set_current_camera("my_camera");
        // 将当前着色器设置为"my_shader"
        world_instance.use_shader("my_shader");

        // 设置gl一些基本东西
        lomeglcall(glClearColor, 0.2F, 0.3F, 0.3F, 1.0F);
        lomeglcall(glViewport, 0, 0, world_instance.get_screen_width(), world_instance.get_screen_height());
        lomeglcall(glEnable, GL_DEPTH_TEST);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // 渲染循环
        while (glfwWindowShouldClose(window) == 0)
        {
            // 计算该帧与上一帧的间隔时间
            lomegl::glfw_utility::calulate_frame_time();
            // 处理用户输入
            lomegl::glfw_utility::process_input(window);

            // 清除之前的颜色缓冲和深度缓存
            lomeglcall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 修改当前着色器的一些uniform变量
            auto color_value = static_cast<float>((sin(glfwGetTime()) / 2.0F) + 0.5F);
            world_instance.get_current_shader().uniform(glUniform4f, "our_color", color_value, color_value, color_value, 1.0F);
            // 将着色器view_mat设置为当前摄像机的观察矩阵
            world_instance.set_current_shader_view_mat();
            // 将着色器projection_mat设置为新的投影矩阵
            world_instance.set_current_shader_projection_mat(glm::radians(fov));

            // 修改每个方块的位置，然后进行绘制
            for (size_t i = 0; i < 10; i++)
            {
                auto&& my_cube = dynamic_cast<lomegl::gl_entity&>(world_instance.get<lomegl::gl_entity>(std::string("my_cube") + std::to_string(i)));
                my_cube.add_angle_pitch_local(glm::radians(0.1F * lomegl::glfw_utility::delta_time));
                // 内部会自动将模型矩阵写入到世界当前着色器中
                my_cube.draw(GL_TRIANGLES);
            }

            // 交换缓冲并处理事件
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
    if (fov >= 1.0F && fov <= 45.0F)
        fov -= static_cast<float>(yoffset);
    if (fov <= 1.0F)
        fov = 1.0F;
    if (fov >= 45.0F)
        fov = 45.0F;
}

void init_resource()
{
    auto&& world_instance = lomegl::gl_world::instance();

    // 加载图片，并设置对应纹理
    auto my_data = lomegl::get_image_from_file(lomegl::from_source_dir("/meizi.jpg"));

    // 纹理1
    auto&& texture1 = world_instance.create<lomegl::gl_texture>("texture1", GL_TEXTURE_2D);
    texture1.active_texture_unit(GL_TEXTURE0)
        .bind()
        .tex_parameteri(GL_TEXTURE_WRAP_S, GL_REPEAT)
        .tex_parameteri(GL_TEXTURE_WRAP_T, GL_REPEAT)
        .tex_parameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        .tex_parameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        .add_image_data_to(0, GL_RGB, my_data.width, my_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_data.data.get())
        .generate_mipmap();

    my_data = lomegl::get_image_from_file(lomegl::from_source_dir("/meizi2.jpg"));

    // 纹理2
    auto&& texture2 = world_instance.create<lomegl::gl_texture>("texture2", GL_TEXTURE_2D);
    texture2.active_texture_unit(GL_TEXTURE1)
        .bind()
        .tex_parameteri(GL_TEXTURE_WRAP_S, GL_REPEAT)
        .tex_parameteri(GL_TEXTURE_WRAP_T, GL_REPEAT)
        .tex_parameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        .tex_parameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        .add_image_data_to(0, GL_RGB, my_data.width, my_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_data.data.get())
        .generate_mipmap();

    my_data.data.reset();

    // 设置方块顶点
    auto&& rect = world_instance.create<lomegl::gl_vertex>("rect");

    rect.bind_this()
        .bind_array_buffer_data(vertices, sizeof(vertices), GL_STATIC_DRAW, _countof(vertices))
        .vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(my_vertex), (void*)offsetof(my_vertex, pos)) // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,performance-no-int-to-ptr)
        .enable_vertex_attrib_array(0)
        .vertex_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(my_vertex), (void*)offsetof(my_vertex, texture_pos)) // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,performance-no-int-to-ptr)
        .enable_vertex_attrib_array(2);

    // 设置和编译着色器程序
    auto&& my_shader = world_instance.create<lomegl::gl_shader>("my_shader");

    my_shader.add_vertex(lomegl::get_content_from_file(lomegl::from_source_dir("/shaders/texture_vertex.glsl")))
        .add_fragment(lomegl::get_content_from_file(lomegl::from_source_dir("/shaders/texture_fragment.glsl")))
        .link_shader()
        .use()
        .uniform(glUniform1i, "texture1", 0)
        .uniform(glUniform1i, "texture2", 1);

    // 创建10个方块实体
    for (size_t i = 0; i < 10; i++)
    {
        auto&& my_cube = world_instance.create<lomegl::gl_entity>(std::string("my_cube") + std::to_string(i));
        my_cube.add_texture("texture1")
            .add_texture("texture2")
            .set_vertex("rect")
            .set_pos(cubePositions[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    // 创建一个相机实体
    world_instance.create<lomegl::gl_entity>("my_camera");
}