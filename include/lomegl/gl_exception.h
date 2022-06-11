#pragma once
#include <glad/glad.h>

#include <cassert>
#include <exception>
#include <sstream>
#include <stdexcept>

#include <lotools/errors.h>

namespace lomegl {

struct gl_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// The follwing macro will throw a 'gl_error' exception if glGetError() does't equals zero
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define lomeglcall(func, ...) lotcall([](auto) {}, [](const char* filename, int line, const char* funcname, auto) {auto error_code = glGetError(); \
    if (error_code != 0) {                                                   \
        std::stringstream out;                                              \
        out << funcname << " (" << filename << ":" << line << ") " << ::lomegl::get_error_content(error_code) << "\n";\
        throw std::runtime_error(out.str());\
        } }, \
    [](auto) { return true; }, func, __VA_ARGS__)

constexpr const char* get_error_content(unsigned int error_code)
{
    switch (error_code)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        return "An unknown error occurred";
    }
}

} // namespace lomegl