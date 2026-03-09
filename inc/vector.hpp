#pragma once
#include <glm/glm.hpp>
#include <format>
#include <iostream>
#include <iterator>

using vec3   = glm::dvec3;
using point3 = glm::dvec3;

[[nodiscard]] inline double dot(const vec3& u, const vec3& v) noexcept {
    return glm::dot(u, v);
}

[[nodiscard]] inline vec3 cross(const vec3& u, const vec3& v) noexcept {
    return glm::cross(u, v);
}

[[nodiscard]] inline vec3 unit_vector(const vec3& v) noexcept {
    return glm::normalize(v);
}

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