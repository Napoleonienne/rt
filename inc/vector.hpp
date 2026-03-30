#pragma once
#define GLM_SWIZZLE_FUNCTION
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <format>
#include <iostream>
#include <iterator>
#include <ranges>



using vec2   = glm::dvec2;

using vec3   = glm::dvec3;
using point3 = glm::dvec3;


constexpr double infinity = std::numeric_limits<double>::infinity();
// Formatting
template <>
struct std::formatter<vec3> : std::formatter<std::string_view> {
    auto format(const vec3& v, format_context& ctx) const {
        return std::format_to(ctx.out(), "{:.0f} {:.0f} {:.0f}", v.x, v.y, v.z);
    }
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    std::format_to(std::ostream_iterator<char>(out), "{:.0f} {:.0f} {:.0f}", v.x, v.y, v.z);
    return out;
}
template <>
struct std::formatter<vec2> : std::formatter<std::string_view> {
    auto format(const vec2& v, format_context& ctx) const {
        return std::format_to(ctx.out(), "{:.0f} {:.0f} ", v.x, v.y);
    }
};

inline std::ostream& operator<<(std::ostream& out, const vec2& v) {
    std::format_to(std::ostream_iterator<char>(out), "{:.0f} {:.0f}", v.x, v.y);
    return out;
}